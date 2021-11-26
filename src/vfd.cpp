/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
    Easy library for Arduino to control a YL-620 VFD inverter
    @file vfd.cpp
    @author Lorenzo Carloni
    @version 0.1a 09/11/2021
*/

#include "vfd.h"


// Private methods

// Checks wether 2 arrays are equal
bool VFD::isEqual(uint8_t* arr1, uint8_t* arr2, int length) {
  for(int i = 0; i < length; i++) {
    if(arr1[i] != arr2[i])
      return false;
  }
  return true;
}

// Calcs CRC of the message, LSB first
uint16_t VFD::calcCrc(uint8_t* buf, int len) {
  uint16_t crc = 0xFFFF;
  
  for (int pos = 0; pos < len; pos++) {
    crc ^= (uint16_t)buf[pos];          // XOR byte into least sig. byte of crc
  
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }
  uint16_t le_crc = (crc>>8) | (crc<<8);
  return le_crc; //crc;  
}


// Sends a command to the VFD command register
VFD_Comm_Errors VFD::sendCommand(VFD_Commands c) {
  return writeRegister(VFD_REGISTER_COMMAND, c);
}


// Writes into a single register
VFD_Comm_Errors VFD::writeRegister(VFD_Registers r, uint16_t value) {
  // request format:
  //  1. address
  //  2. operation (write register (6))
  //  3. Register command high
  //  4. register command low
  //  5. data high (0)
  //  6. data low (c)
  //  7. crc low
  //  8. crc high

  while(comm_stream->available()) comm_stream->read(); // clear receive buffer!

  // Creating request
  uint8_t request[8] = {address, 0x06, (uint8_t)(r >> 8), (uint8_t)r, (uint8_t)(value >> 8), (uint8_t)value, 0x00, 0x00};
  uint16_t crc = calcCrc(request, 6); // calculating crc on 6 bytes
  request[6] = (uint8_t)(crc >> 8); // adding crc to the request
  request[7] = (uint8_t)crc;


  if(comm_pin != -1) { // if using a half duplex TTL converter put in trensimt mode
    digitalWrite(comm_pin, CP_TRANSMIT_LEVEL);
  }

  delay(min_timing); // 3.5 char time delay
  comm_stream->write(request, 8); // send request
  comm_stream->flush(); // waiting end of transmission

  if(comm_pin != -1) { // getting back to receive mode if needed
    digitalWrite(comm_pin, CP_RECEIVE_LEVEL);
  }
  
  // getting response
  long started = millis();
  while(comm_stream->available() < 8 && millis()-started < COMM_TIMEOUT_TIME); // wait response
  // on write register response should be echo of request...

  if(comm_stream->available() != -1 && comm_stream->available() < 8) { // data size mismatch
    last_error = VFD_COMM_ERROR_UNEXPECTED_RESPONSE;
    return VFD_COMM_ERROR_UNEXPECTED_RESPONSE; // unexpected result...
  }

  if(comm_stream->available() == -1) {  // if nothing in buffer timeout happened
    last_error = VFD_COMM_ERROR_NO_RESPONSE;
    return VFD_COMM_ERROR_NO_RESPONSE; // nothing in buffer...
  }
  
  // get the data
  uint8_t response[8];
  int i = 0;
  while(comm_stream->available()) response[i++] = comm_stream->read();

  // calc crc on the response
  uint16_t response_crc = calcCrc(response, 6); // calc on all bytes except 2 CRC bytes
  if((uint8_t)(response_crc >> 8) != response[6] || (uint8_t)response_crc != response[7]) { // CRC mismatch
    last_error = VFD_COMM_ERROR_WRONG_CRC;
    return VFD_COMM_ERROR_WRONG_CRC;
  }

  if(response[0] != address) {  // is the device who's calling our device?
    last_error = VFD_COMM_ERROR_WRONG_DEVICE;
    return VFD_COMM_ERROR_WRONG_DEVICE;
  }

  if(!isEqual(response, request, 8)) {  // return response should be equal to request
    last_error = VFD_COMM_ERROR_GENERIC;
    return VFD_COMM_ERROR_GENERIC;
  }

  // transmission correct
  last_error = VFD_COMM_SUCCESS;
  return VFD_COMM_SUCCESS;
}

// Reads from multiple registers at once
VFD_Comm_Errors VFD::readMultipleRegisters(VFD_Registers start_register, uint8_t num_register, uint16_t store_arr[]) {
  // request format:
  //  1. address
  //  2. operation (read registers (3))
  //  3. First register address high
  //  4. First register address low
  //  5. Number of register to read high
  //  6. Number of register to read low
  //  7. crc low
  //  8. crc high
  while(comm_stream->available()) comm_stream->read(); // clear read buffer!

  uint8_t request[8] = {address, 0x03, (uint8_t)(start_register >> 8), (uint8_t)start_register, (uint8_t)(num_register >> 8), (uint8_t)num_register, 0x00, 0x00};
  uint16_t crc = calcCrc(request, 6); // calculating crc on 6 bytes
  request[6] = (uint8_t)(crc >> 8); // adding crc to the request
  request[7] = (uint8_t)crc;

  if(comm_pin != -1) { // if using a half duplex TTL converter put in transimt mode
    digitalWrite(comm_pin, CP_TRANSMIT_LEVEL);
  }

  delay(min_timing); // 3.5 char time delay
  comm_stream->write(request, 8);
  comm_stream->flush(); // waiting end of transmission

  if(comm_pin != -1) { // getting back to receive mode if needed
    digitalWrite(comm_pin, CP_RECEIVE_LEVEL);
  }

  // response should be
  //  0 - address
  //  1 - function (0x03)
  //  2 - byte count (1 in this case)
  //  n - Data High
  //  n+1 - Data Low
  //  n+2 - Data High
  //  n+3 - Data Low
  //  ......
  //  n+x - Data High/Low
  //  n+x+1 - CRC Low
  //  n+x+2 - CRC High
  // so we have 3 bytes "common header" + 2*num_register byte of data + 2 byte CRC
  // => 5 + 2*num_register bytes to read!
  int byte_to_read = 5+2*num_register;

  long started = millis();
  while(comm_stream->available() < byte_to_read && millis()-started < COMM_TIMEOUT_TIME); // wait response

  if(comm_stream->available() == 0) { // no data received
    last_error = VFD_COMM_ERROR_NO_RESPONSE;
    return last_error; // nothing in buffer...
  }

  /*if(comm_stream->available() != 0 && comm_stream->available() < byte_to_read) { // data size unexpected
    last_error = VFD_COMM_ERROR_UNEXPECTED_RESPONSE;
    return last_error; // unexpected result...
  }*/

  // get the data
  uint8_t response[byte_to_read];
  int i = 0;
  while(comm_stream->available()) response[i++] = comm_stream->read();
		
  // calc crc on the response
  uint16_t response_crc = calcCrc(response, (byte_to_read-2)); // calc on all bytes except 2 CRC bytes
  if((uint8_t)(response_crc >> 8) != response[(byte_to_read-2)] || (uint8_t)response_crc != response[(byte_to_read-1)]) { // CRC mismatch
    last_error = VFD_COMM_ERROR_WRONG_CRC;
    return last_error;
  }

  if(response[0] != address) {  // is the device who's calling our device?
    last_error = VFD_COMM_ERROR_WRONG_DEVICE;
    return 0;
  }

  // datas are stored as 2 bytes per register, MSB first
  // cycle throught datas and store them in array
  for(int i = 0; i < num_register; i++) {
    store_arr[i] = response[2*i+3] << 8;
    store_arr[i] |= response[2*i+4];
  }
  last_error = VFD_COMM_SUCCESS;  // no error to report
  return last_error;
}


// Reads a single register
uint16_t VFD::readRegister(VFD_Registers r) {
  // request format:
  //  1. address
  //  2. operation (read single register (3))
  //  3. First register address high
  //  4. First register address low
  //  5. Number of register to read high (int16) (0)
  //  6. Number of register to read low (we want only 1 so...) (1)
  //  7. crc low
  //  8. crc high
  while(comm_stream->available()) comm_stream->read(); // clear read buffer!

  uint8_t request[8] = {address, 0x03, (uint8_t)(r >> 8), (uint8_t)r, 0x00, 0x01, 0x00, 0x00};
  uint16_t crc = calcCrc(request, 6); // calculating crc on 6 bytes
  request[6] = (uint8_t)(crc >> 8); // adding crc to the request
  request[7] = (uint8_t)crc;

  if(comm_pin != -1) { // if using a half duplex TTL converter put in transimt mode
    digitalWrite(comm_pin, CP_TRANSMIT_LEVEL);
  }

  delay(min_timing); // 3.5 char time delay
  comm_stream->write(request, 8);
  comm_stream->flush(); // waiting end of transmission

  if(comm_pin != -1) { // getting back to receive mode if needed
    digitalWrite(comm_pin, CP_RECEIVE_LEVEL);
  }

  // response should be
  //  0 - address
  //  1 - function (0x03)
  //  2 - byte count (1 in this case)
  //  3 - Data High
  //  4 - Data Low
  //  5 - CRC Low
  //  6 - CRC High
  long started = millis();
  while(comm_stream->available() < 7 && millis()-started < COMM_TIMEOUT_TIME); // wait response

  if(comm_stream->available() == 0) { // no data received
    last_error = VFD_COMM_ERROR_NO_RESPONSE;
    return 0; // nothing in buffer...
  }

  if(comm_stream->available() != 0 && comm_stream->available() < 7) { // data size unexpected
    last_error = VFD_COMM_ERROR_UNEXPECTED_RESPONSE;
    return 0; // unexpected result...
  }
  
  // get the data
  uint8_t response[7];
  int i = 0;
  while(comm_stream->available()) response[i++] = comm_stream->read();

  // calc crc on the response
  uint16_t response_crc = calcCrc(response, 5); // calc on 5 bytes (avoid received CRC)
  if((uint8_t)(response_crc >> 8) != response[5] || (uint8_t)response_crc != response[6]) { // CRC mismatch
    last_error = VFD_COMM_ERROR_WRONG_CRC;
    return 0;
  }

  if(response[0] != address) {  // is the device who's calling our device?
    last_error = VFD_COMM_ERROR_WRONG_DEVICE;
    return 0;
  }
  // datas are stored as 2 bytes, MSB first
  uint16_t value = (response[3] << 8) | response[4];
  last_error = VFD_COMM_SUCCESS;  // no error to report
  return value;
}




// Public methods

// class constructor(s)
//VFD::VFD(uint8_t _address, HardwareSerial &_comm_stream) {
VFD::VFD(uint8_t _address, Stream& _comm_stream) {
  comm_stream = &_comm_stream;
  address = _address;
  baud_rate = 9600;
  comm_pin = -1;
  last_error = VFD_COMM_SUCCESS;
  direction = running = false;

  min_timing = 4; // (uint8_t)floor(1000/(baud/10)*3.5f);
}
//VFD::VFD(uint8_t _address, HardwareSerial &_comm_stream, int baud) {
VFD::VFD(uint8_t _address, Stream& _comm_stream, int baud) {
  comm_stream = &_comm_stream;
  address = _address;
  baud_rate = baud;
  comm_pin = -1;
  last_error = VFD_COMM_SUCCESS;
  direction = running = false;

  min_timing = (uint8_t)floor(1000/(baud/10)*3.5f);
}
//VFD::VFD(uint8_t _address, HardwareSerial &_comm_stream, int baud, uint8_t _comm_pin) {
VFD::VFD(uint8_t _address, Stream& _comm_stream, int baud, uint8_t _comm_pin) {
  comm_stream = &_comm_stream;
  address = _address;
  baud_rate = baud;
  comm_pin = _comm_pin;
  last_error = VFD_COMM_SUCCESS;
  direction = running = false;

  min_timing = (uint8_t)floor(1000/(baud/10)*3.5f);
}

// Class init routine
void VFD::begin() {
  // if we have a half-duplex comm we need to initialize pin direction...
  if(comm_pin != -1) {
    pinMode(comm_pin, OUTPUT);
    digitalWrite(comm_pin, CP_RECEIVE_LEVEL); // ... and get in receive mode
  }

  // get fwd/bwd direction data etc...
  // todo...
}

// Sets VFD Frequency
void VFD::setSpeed(float speed) {
  writeRegister(VFD_REGISTER_FREQUENCY, (uint16_t)(speed*10));
}

// start the motor
void VFD::run() {
  if(sendCommand(VFD_COMMAND_START) == VFD_COMM_SUCCESS) running = true;;
}

// stop the motor
void VFD::stop() {
  if(sendCommand(VFD_COMMAND_STOP) == VFD_COMM_SUCCESS) running = false;
}

// start the motor in forward dir
void VFD::runForward() {
  if(sendCommand(VFD_COMMAND_START_FORWARD) == VFD_COMM_SUCCESS) direction = true;
}

// start the motor in backward dir
void VFD::runBackward() {
  if(sendCommand(VFD_COMMAND_START_BACKWARD) == VFD_COMM_SUCCESS) direction = false;
}

// changes direction of the motor
void VFD::changeDirection() {
  if(sendCommand(VFD_COMMAND_CHANGE_DIRECTION) == VFD_COMM_SUCCESS) direction = !direction;
}

// set forward direction
void VFD::setForward() {
  if(sendCommand(VFD_COMMAND_FORWARD) == VFD_COMM_SUCCESS) direction = true;
}

// set backward direction
void VFD::setBackward() {
  if(sendCommand(VFD_COMMAND_BACKWARD) == VFD_COMM_SUCCESS) direction = false;
}

// reset VFD error
void VFD::resetError() {
  sendCommand(VFD_COMMAND_RESET_ERROR);
}

// reset all VFD errors
void VFD::resetAllErrors() {
  sendCommand(VFD_COMMAND_RESET_ALL_ERRORS);
}

// gets VFD error 
VFD_Errors VFD::getError() {
  return readRegister(VFD_REGISTER_ERROR_CODE);
}

// gets frequency the vfd is at
float VFD::getRunFreq() {
  return readRegister(VFD_REGISTER_RUN_FREQ)/10.0f;
}

// Get unique CPU ID
uint16_t VFD::getCpuID() {
  cpu_id = readRegister(VFD_REGISTER_UNIQUE_ID);
  return cpu_id;
}

// gets the frequency the vfd is trying to reach
float VFD::getAimFreq() {
  return readRegister(VFD_REGISTER_AIM_FREQ)/10.0f;
}

// gets the output current from the VFD
float VFD::getOutCurrent() {
  return readRegister(VFD_REGISTER_OUT_CURR); // need multiply?
}

// gets the output voltage from the VFD
float VFD::getRunVoltage() {
  return readRegister(VFD_REGISTER_RUN_VOLT); // need multiply?
}

// gets the bus voltage from the VFD
float VFD::getBusVoltage() {
  return readRegister(VFD_REGISTER_BUS_VOLT); // need multiply?
}

// gets the acceleration time in mS
float VFD::getAccelTime() {
  return readRegister(VFD_REGISTER_ACCEL_TIME)/10.0f; // need multiply?
}

// sets acceleration time in mS
void VFD::setAccelTime(float time) {
  return writeRegister(VFD_REGISTER_ACCEL_TIME, (uint16_t)(time*10));
}

// gets the deceleration time in mS
float VFD::getDecelTime() {
  return readRegister(VFD_REGISTER_DECEL_TIME)/10.0f; // need multiply?
}

// sets acceleration time in mS
void VFD::setDecelTime(float time) {
  return writeRegister(VFD_REGISTER_DECEL_TIME, (uint16_t)(time*10));
}

// gets last VFD communication error
char* VFD::lastCommError() {
  switch(last_error) {
    case VFD_COMM_SUCCESS:
      return "No error";
    case VFD_COMM_ERROR_WRONG_CRC:
      return "CRC mismatch";
    case VFD_COMM_ERROR_NO_RESPONSE:
      return "No response";
    case VFD_COMM_ERROR_UNEXPECTED_RESPONSE:
      return "Unexpected response";
    case VFD_COMM_ERROR_GENERIC:
      return "Generic error";
    case VFD_COMM_ERROR_WRONG_DEVICE:
      return "Wrong device responded";
    default:
      return "Unknown";
  }
}

// checks if motor is running
int VFD::status() {
  if(getRunFreq() == 0) return 0; // stopped
 // if(readRegister(VFD_REGISTER_CURRENT_ACCEL_TIME) < readRegister(VFD_REGISTER_ACCEL_TIME)) return 2;
 // if(readRegister(VFD_REGISTER_CURRENT_DECEL_TIME) < readRegister(VFD_REGISTER_DECEL_TIME)) return 3;
  return 1;
}

// checks if motor is in forward direction
bool VFD::isForward() {
  uint16_t command = readRegister(VFD_REGISTER_COMMAND);
  return (command >> 4) & 0x01;
}

// checks if motor is in backward direction
bool VFD::isBackward() {
  uint16_t command = readRegister(VFD_REGISTER_COMMAND);
  return (command >> 5) & 0x01;
}

// gets value of a parameter
uint16_t VFD::getParameter(uint8_t section, uint8_t param) {
  uint16_t reg = (section << 8) | param;
  return readRegister(reg);
}

// sets value of a parameter
VFD_Comm_Errors VFD::setParameter(uint8_t section, uint8_t param, uint16_t value) {
  uint16_t reg = (section << 8) | param;
  return writeRegister(reg, value);
}

// updates running parametes, to be reched via "fetch" methods
VFD_Comm_Errors VFD::update() {
  // we're gonna read 34 registers, skip those we don't need and then map values we need to our running variables
  uint16_t read_data[34];
  VFD_Comm_Errors error = readMultipleRegisters(VFD_REGISTER_COMMAND, 34, read_data); // VFD_REGISTER_COMMAND is the lower value address we need
  if(error != VFD_COMM_SUCCESS) return error; // something bad happened! the user will have to figure out what

  // not all the data we need is stored in readed registers, just "bind" the variables we need
  out_current = read_data[VFD_REGISTER_OUT_CURR-0x2000];
  run_volt = read_data[VFD_REGISTER_RUN_VOLT-0x2000];
  bus_volt = read_data[VFD_REGISTER_BUS_VOLT-0x2000];
  operating_command = read_data[0];
  
  // these are stored in VFD as 10x the value
  accel_time = read_data[VFD_REGISTER_ACCEL_TIME-0x2000];
  decel_time = read_data[VFD_REGISTER_DECEL_TIME-0x2000];
  aim_freq = read_data[VFD_REGISTER_AIM_FREQ-0x2000]/10.0f;
  run_freq = read_data[VFD_REGISTER_RUN_FREQ-0x2000]/10.0f;
  cpu_id = read_data[VFD_REGISTER_UNIQUE_ID-0x2000];
  /// True = FWD - False = BWD
  direction = (operating_command >> 4) & 1;
  running = (operating_command >> 1) & 1;
}

// Retrieves Acceleration time from library
float VFD::fetchAccelTime() {
  return accel_time;
}

// Retrieves Deceleration time from library
float VFD::fetchDecelTime() {
  return decel_time;
}

// Retrieves aim frequency from library
float VFD::fetchAimFrequency() {
  return aim_freq;
}

// Retrieves run frequency from library
float VFD::fetchRunFrequency() {
  return run_freq;
}

// Retrieves output current from library
uint16_t VFD::fetchOutCurrent() {
  return out_current;
}

// Retrieves running voltage from library
uint16_t VFD::fetchRunVoltage() {
  return run_volt;
}

// Retrieves bus voltage from library
uint16_t VFD::fetchBusVoltage() {
  return bus_volt;
}

// Retrieves command register value
uint16_t VFD::fetchOperatingCommand() {
  return operating_command;
}

// Retrieves unique id
uint16_t VFD::fetchCPUId() {
  return cpu_id;
}

// Retrieves direction of the VFD
bool VFD::fetchDirection() {
  return direction;
}

// Retrieves if the VFD is running forward
bool VFD::fetchForward() {
  return direction == true;
}

// Retrieves if the VFD is running backward
bool VFD::fetchBackward() {
  return direction == false;
}

// Retrieves if the VFD is running
bool VFD::fetchRunning() {
  return running;
}
