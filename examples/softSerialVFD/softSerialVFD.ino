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

/*
    This example shows how to just set speed (goes 10 to 30Hz and then back) and checks errors.
    This example uses an arduino UNO (or duemilanove or mini) and uses SoftwareSerial as serial interface.

    Using a MAX485 module for communication.

    Connections as follow:

    Arduino PIN         -   MAX485 module
    +5V                 -   VCC
    GND                 -   GND
    D5                  -   DE (connected togheter with RE)
    D5                  -   RE (connected togheter with DE)
    D6 (SoftSerial TX)  -   DI
    D7 (SoftSerial RX)  -   RO


*/
#include <YL620-Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial rsSerial(7, 6); // RX, TX

/*
    Make sure VFD parameters are set as follow:
    P00.01 => 3 (allows command over RS485)
    P03.00 => 5 (38400 baud serial "speed")
    P03.01 => 10 (this is modbus address of VFD)
    P03.02 => 2 (setting data transfer format, this is SERIAL_8N1, the only supported by softwareserial)
    P07.08 => 5 (VFD frequency set via RS485)
*/

/*
  Here we're creating the object for the library.
  Parameters are:
  VFD inverter(address, serial, baudrate, comm_pin)
    - address: address of the VFD (P03.01). In this example the address of VFD is 10
    - serial: we're using a UNO board and softwareSerial (rsSerial)
    - baudrate: communication speed, setted in parameter P03.00 (in this case is 38400 baud)
    - comm_pin: on MAX485 there are pins DE and RE to set the module in transmit or receive mode.
                in this case we're using just pin D5 to switch between both modes (pin D5 connected to BOTH RE and DE pins on the module)
*/
const int comm_pin = 5;  // using pin D15 for commutating between receive and transmit mode in MAX485
VFD inverter(10, rsSerial, 38400, comm_pin);

float speed = 10.0;

void setup() {
  Serial.begin(9600); // Start USB serial, so you can print to serial monitor

  // rsSerial is our RS485 serial. The VFD parameter P03.02 sets the data transfert format
  // the only supported data transfer format in softwareserial right now is 8N1.
  // i got troubles using 8N1 (default arduino serial data transfer format, value 2 in parameter P03.02), but others found no problem with it, so good to take a shot.
  // i believe that's due to the device using a 2nd stop bit (see https://modbus.org/docs/Modbus_over_serial_line_V1_02.pdf page 12 sec. 2.5.1)
  rsSerial.begin(38400);

  // now we begin the library
  inverter.begin();


}

void loop() {
  // set speed on the VFD
  inverter.setSpeed(speed);

  speed = speed + 0.1; // increase speed
  if(speed > 30.0) { // if speed is too high get back to 10
    speed = 10.0;
  }

  // check if we have any error on the VFD
  VFD_Errors err = inverter.getError();
  if(err != VFD_ERROR_NO_ERROR) {
    // print the error out
    Serial.println(err);

    // after 5 seconds reset the vfd
    delay(5000);
    inverter.resetAllErrors();
  }

  // just a little delay to 
  delay(500);

}
