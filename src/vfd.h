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
    It uses a fast implementation of MODBUS RTU to control the inverter.
    @file vfd.h
    @author Lorenzo Carloni
    @version 0.1a 09/11/2021
*/

#ifndef _VFD_H_
#define _VFD_H_


#include <Arduino.h>

/// Pin level of transmit mode
#define CP_TRANSMIT_LEVEL HIGH
/// Pin level of receive mode
#define CP_RECEIVE_LEVEL  LOW

#ifndef COMM_TIMEOUT_TIME
  /// Default timeout time
  #define COMM_TIMEOUT_TIME 100
#endif


/// List of VFD accepted commands
enum VFD_Commands : uint8_t {
  VFD_COMMAND_START = 0b00000010,  ///< Start inverter command
  VFD_COMMAND_STOP = 0b00000001,   ///< Stop inverter command
  VFD_COMMAND_START_FORWARD = 0b00010010,  ///< Set forward direction and start
  VFD_COMMAND_START_BACKWARD = 0b00100010,   ///< Set backward direction and start
  VFD_COMMAND_START_CHANGE_DIRECTION = 0b00110010,   ///< Changes VFD direction
  VFD_COMMAND_FORWARD = 0b00010000,  ///< Set forward direction
  VFD_COMMAND_BACKWARD = 0b00100000,   ///< Set backward direction
  VFD_COMMAND_CHANGE_DIRECTION = 0b00110000,   ///< Change direction
  VFD_COMMAND_RESET_ERROR = 0b01000000,  ///< Reset VFD last error
  VFD_COMMAND_RESET_ALL_ERRORS = 0b10000000,   ///< Reset VFD all errors
};

/// List of VFD register addresses
enum VFD_Registers : uint16_t {
  VFD_REGISTER_COMMAND = 0x2000, ///< Command register (start, stop, error reset, direction, ecc)
  VFD_REGISTER_FREQUENCY = 0x2001, ///< Frequency set of inverter (rs485 frequency source, see param P07.08)
  VFD_REGISTER_ACCEL_TIME = 0x2002,  ///< RS485 Acceleration time
  VFD_REGISTER_DECEL_TIME = 0x2003, ///< RS485 Deceleration time register
  VFD_REGISTER_ERROR_CODE = 0x2008, ///< Error flag (Read only)
  VFD_REGISTER_AIM_FREQ = 0x200A, ///< Register of target frequency (Read only)
  VFD_REGISTER_RUN_FREQ = 0x200B, ///< Register of the frequency the vfd is at (Read only)
  VFD_REGISTER_OUT_CURR = 0x200C, ///< Register of the output current (Read only)
  VFD_REGISTER_RUN_VOLT = 0x200D, ///< Running voltage register (Read only)
  VFD_REGISTER_BUS_VOLT = 0x200E, ///< Bus voltage register (Read only)
  VFD_REGISTER_CURRENT_ACCEL_TIME = 0x2011, ///< Current acceleration time (Read only)
  VFD_REGISTER_CURRENT_DECEL_TIME = 0x2012, ///< Current deceleration time (Read only)
  VFD_REGISTER_UNIQUE_ID = 0x2021,  ///< CPU Unique ID attributecode 
};

/// List of VFD Errors
enum VFD_Errors : uint16_t {
  VFD_ERROR_NO_ERROR = 0x00,  ///< No error detected
  VFD_ERROR_LOW_VOLTAGE = 16,//0,  ///< Power failure, supply voltage is above 140VAC (error 0 in manual)
  VFD_ERROR_HIGH_VOLTAGE = 1, ///< Supply voltage is over 290VAC, internal bus voltage too high
  VFD_ERROR_GREAT_CURRENT = 2,  ///< Great current
  VFD_ERROR_EXTERNAL_PWM = 3, ///< Fault in external PWM circuit
  VFD_ERROR_IPM = 4,  ///< IPM Alarm (short-circuit)
  VFD_ERROR_EXTERNAL = 0x20, ///< External input fault signal (error 5 in manual)
  VFD_ERROR_INTERNAL_STORAGE = 6, ///< Fault in internal data storage
  VFD_ERROR_OVER_TEMPERATURE = 7, ///< Over temperature
  VFD_ERROR_NTC_FAULT = 8,  ///< Fault in temperature measurement NTC
  VFD_ERROR_POWEROFF_PROMPT = 0x0400, ///< Power off prompt (error 9 in manual)
  VFD_ERROR_RS485 = 11, ///< RS485 dropped line error, stop
  VFD_ERROR_PARAMETER = 12, ///< Parameter error
  VFD_ERROR_OVERHEATING = 15, ///< Motor overheating
};

/// List of library communication error
enum VFD_Comm_Errors : uint8_t {
  VFD_COMM_SUCCESS=0, ///< Success in communication, no error
  VFD_COMM_ERROR_WRONG_CRC, ///< CRC calculation differs..
  VFD_COMM_ERROR_NO_RESPONSE, ///< No response in communication timeout time
  VFD_COMM_ERROR_UNEXPECTED_RESPONSE, ///< Response byte count differs from what expected (might need to increase timeout time)
  VFD_COMM_ERROR_GENERIC, ///< When none of the above..
  VFD_COMM_ERROR_WRONG_DEVICE,
};


/**
 * @brief VFD class for inverter control
 */
class VFD {
  /// MODBUS address of inverter (param P03.01)
  uint8_t address;

  /// Stream class used for communication
  //HardwareSerial* comm_stream;
  Stream& comm_stream;  

  /// Arduino pin for half-duplex converter. Setted at -1 if not needed
  int comm_pin;

  /// MODBUS RTU requires a 3.5char timing space before comm. Calculating how many ms and storing them here
  uint8_t min_timing;

  /// Communication baud (Param P03.00)
  uint8_t baud_rate;

  /**
     * @defgroup runparam Running parameters
     * These parameters are kept in the library, you can access them via the "fetch" methods
     * they are updated via the update() method
     * @see update();
     * @{
  */
  float accel_time;  ///< Acceleration time
  float decel_time; ///< Deceleration time
  float aim_freq;  ///< Frequency the inverter is trying to reach
  float run_freq; ///< Frequency the inverter is now running at
  uint16_t out_current; ///< Current to the motor
  uint16_t run_volt;  ///< Running voltage
  uint16_t bus_volt;  ///< VFD Bus voltage
  uint16_t operating_command; ///< VFD command register status
  uint16_t cpu_id;  ///< Unique VFD ID
  bool direction; ///< True = FWD - False = BWD
  bool running; ///< Is the motor running?
  /** @}*/

  /// Last error, triggered from VFD_REGISTER_ERROR_CODE
  VFD_Errors last_vfd_error;
  
  /// Last library error
  VFD_Comm_Errors last_error;


  /**
     * @brief Checks wether 2 arrays are equal
     * @param arr1 first array to compare
     * @param arr2 second array to compare
     * @param length size of the arrays
     * @return true if equal, false otherwise
  */
  bool isEqual(uint8_t* arr1, uint8_t* arr2, int length);

  /**
     * @brief Calcs CRC of the message, LSB first
     * @param buf buffer of the request
     * @param len length of the buffer
     * @return 2 byte CRC (LSB fisrt)
  */
  uint16_t calcCrc(uint8_t* buf, int len);

  /**
     * @brief Sends a command (writing on the command register)
     * @param c command to send
     * @return error or VFD_COMM_SUCCESS if ok
  */
  VFD_Comm_Errors sendCommand(VFD_Commands c);

  /**
     * @brief Write in to a single register
     * @param r register to write into
     * @param value 2 byte value to write into register
     * @return error or VFD_COMM_SUCCESS if ok
  */
  VFD_Comm_Errors writeRegister(VFD_Registers r, uint16_t value);

  /**
     * @brief Reads multiple registers at once
     * @param start_register Address of the first register to read
     * @param num_register Number of registers to read
     * @param store_arr Pointer to the array wich will contain the datas
     * @return Error or VFD_COMM_SUCCESS if ok
  */
  VFD_Comm_Errors readMultipleRegisters(VFD_Registers start_register, uint8_t num_register, uint16_t store_arr[]);

  /**
     * @brief Reads from a register
     * @param r register to read
     * @return 2 byte register content
  */
  uint16_t readRegister(VFD_Registers r);


public:
  /**
     * Create a new VFD object. Required params address and comm_stream. If no baud specified used 9600.
     * If no comm_pin specified doesn't switch during comm (use with full-duplex adapter).
     * @brief Constructor.
     * @param _address address of the VFD (param P03.01).
     * @param _comm_stream communication stream (Serial1, Serial, VirtualSerial, ecc...).
     * @see VFD(uint8_t _address, Stream &_comm_stream, int baud);
     * @see VFD(uint8_t _address, Stream &_comm_stream, int baud, uint8_t _comm_pin);
  */
  //VFD(uint8_t _address, HardwareSerial& _comm_stream);
  VFD(uint8_t _address, Stream& _comm_stream);
  /**
     * Create a new VFD object. Required params address and comm_stream. If no baud specified used 9600.
     * If no comm_pin specified doesn't switch during comm (use with full-duplex adapter).
     * @brief Constructor.
     * @param _address address of the VFD (param P03.01).
     * @param _comm_stream communication stream (Serial1, Serial, VirtualSerial, ecc...).
     * @param baud VFD communication baudrate (param P03.00).
     * @see VFD(uint8_t _address, Stream &_comm_stream);
     * @see VFD(uint8_t _address, Stream &_comm_stream, int baud, uint8_t _comm_pin);
  */
  //VFD(uint8_t _address, HardwareSerial& _comm_stream, int baud);
  VFD(uint8_t _address, Stream& _comm_stream, int baud);
  /**
     * Create a new VFD object. Required params address and comm_stream. If no baud specified used 9600.
     * If no comm_pin specified doesn't switch during comm (use with full-duplex adapter).
     * @brief Constructor.
     * @param _address address of the VFD (param P03.01).
     * @param _comm_stream communication stream (Serial1, Serial, VirtualSerial, ecc...).
     * @param baud VFD communication baudrate (param P03.00).
     * @param _comm_pin Arduino pin for commutating trasmit/receive mode.
     * @see VFD(uint8_t _address, Stream &_comm_stream);
     * @see VFD(uint8_t _address, Stream &_comm_stream, int baud);
  */
  //VFD(uint8_t _address, HardwareSerial& _comm_stream, int baud, uint8_t _comm_pin);
  VFD(uint8_t _address, Stream& _comm_stream, int baud, uint8_t _comm_pin);

  /**
     * @brief First call for pin settings
  */
  void begin();

  /**
     * @brief Sets frequency on the VFD
     * @param speed float of speed (max 1 decimal unit)
  */
  void setSpeed(float speed);

  /**
     * @brief Starts the motor
  */
  void run();

  /**
     * @brief Stops the motor
  */
  void stop();

  /**
     * @brief STARTS THE MOTOR setting forward rotation
  */
  void runForward();

  /**
     * @brief STARTS THE MOTOR setting backward rotation
  */
  void runBackward();

  /**
     * @brief changes direction of the motor (no matter if running or not ?)
  */
  void changeDirection();

  /**
     * @brief changes direction of the motor to forward (no matter if running or not ?)
  */
  void setForward();

  /**
     * @brief changes direction of the motor to backward (no matter if running or not ?)
  */
  void setBackward();

  /**
     * @brief resets last VFD error
  */
  void resetError();

  /**
     * @brief resets all VFD error
  */
  void resetAllErrors();

  /**
     * @brief returns the VFD error
     * @return code of the error
  */
  VFD_Errors getError();

  /**
     * @brief get actual running frequency
     * @return running frequency in Hz
  */
  float getRunFreq();

  /**
     * @brief get Cpu unique id of the VFD
     * @return raw cpuid data
  */
  uint16_t getCpuID();

  /**
     * @brief get target running frequency
     * @return running frequency in Hz
  */
  float getAimFreq();

  /**
     * @brief get actual output current
     * @return out current in Amperes
  */
  float getOutCurrent();

  /**
     * @brief get actual running voltage
     * @return out voltage in Volts
  */
  float getRunVoltage();

  /**
     * @brief Get actual bus voltage
     * @return Bus voltage in Volts
  */
  float getBusVoltage();

  /**
     * @brief Get actual acceleration time
     * @return Acceleration time in ms
  */
  float getAccelTime();

  /**
     * @brief Sets acceleration time
     * @param time Acceleration time in ms
  */
  void setAccelTime(float time);

  /**
     * @brief Get actual deceleration time
     * @return Deceleration time in ms
  */
  float getDecelTime();

  /**
     * @brief Sets deceleration time
     * @param time Deceleration time in ms
  */
  void setDecelTime(float time);

  /**
     * @brief Get last library (communication) error
     * @return Human readable last error (C-style string)
  */
  char* lastCommError();

  /**
     * @brief Gets if inverter is running
     * @return true if running, false otherwise
  */
  bool isRunning();

  /**
     * @brief Gets if inverter is in forward direction
     * @return true if forward, false otherwise
  */
  bool isForward();

  /**
     * @brief Gets if inverter is in backward direction
     * @return true if backward, false otherwise
  */
  bool isBackward();

  /**
     * @brief Get a parameter of the inverter
     * @param section parameter section (es. P01.05 section is 01)
     * @param param parameter (es. P01.05 param is 05)
     * @return value of the stored register
  */
  uint16_t getParameter(uint8_t section, uint8_t param);

  /**
     * @brief Set a parameter of the inverter
     * @param section parameter section (es. P01.05 section is 01)
     * @param param parameter (es. P01.05 param is 05)
     * @param value value to set
     * @return communication error enum
  */
  VFD_Comm_Errors setParameter(uint8_t section, uint8_t param, uint16_t value);

  /**
     * @brief Retrieves all the running parameter of the inverter
     * @return communication error enum
  */
  VFD_Comm_Errors update();

  /**
     * @brief Retrieves Acceleration time from library
     * @return Acceleration time in ms
     * @see update()
  */
  float fetchAccelTime();

  /**
     * @brief Retrieves Deceleration time from library
     * @return Deceleration time in ms
     * @see update()
  */
  float fetchDecelTime();

  /**
     * @brief Retrieves aim frequency from library
     * @return Frequency vfd is trying to reach
     * @see update()
  */
  float fetchAimFrequency();

  /**
     * @brief Retrieves run frequency from library
     * @return Frequency vfd is running at
     * @see update()
  */
  float fetchRunFrequency();

  /**
     * @brief Retrieves output current from library
     * @return Output current
     * @see update()
  */
  uint16_t fetchOutCurrent();

  /**
     * @brief Retrieves running voltage from library
     * @return Output voltage
     * @see update()
  */
  uint16_t fetchRunVoltage();

  /**
     * @brief Retrieves bus voltage from library
     * @return Bus voltage
     * @see update()
  */
  uint16_t fetchBusVoltage();

  /**
     * Fetching operating command register status. Need to see if useful after testing or remove it...
     * @brief Retrieves command register value
     * @return command register value
     * @see update()
  */
  uint16_t fetchOperatingCommand();

  /**
     * @brief Retrieves unique id
     * @return Raw cpu id
     * @see update()
  */
  uint16_t fetchCPUId();

  /**
     * @brief Retrieves direction of the VFD
     * @return TRUE if running forward - FALSE if running backward
     * @see update()
  */
  bool fetchDirection();

  /**
     * @brief Retrieves if the VFD is running forward
     * @return TRUE if running forward - FALSE if not
     * @see update()
  */
  bool fetchForward();

  /**
     * @brief Retrieves if the VFD is running backward
     * @return TRUE if running backward - FALSE if not
     * @see update()
  */
  bool fetchBackward();

  /**
     * @brief Retrieves if the VFD is running
     * @return TRUE if running - FALSE if stopped
     * @see update()
  */
  bool fetchRunning();

};


#endif  // _VFD_H_