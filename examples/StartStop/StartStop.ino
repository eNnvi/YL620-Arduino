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
    This example shows how to start and stop the VFD YL620 via YL620-Arduino library.
    This example uses an arduino MEGA board (multiple serial).

    Using a MAX485 module for communication.

    Connections as follow:

    Arduino PIN       -   MAX485 module
    +5V               -   VCC
    GND               -   GND
    D15               -   DE (connected togheter with RE)
    D15               -   RE (connected togheter with DE)
    D16 (Serial2 TX)  -   DI
    D17 (Serial2 RX)  -   RO

    WARNING!!!!!!
    This sketch starts the VFD, so if you have any motor attached to it it will run.
    Make sure you're running in a safe environment.
    I'm assuming no responsibility for any damage to people or things that can happen when running this sketch!

*/
#include <YL620-Arduino.h>

/*
    Make sure VFD parameters are set as follow:
    P00.01 => 3 (allows command over RS485)
    P03.00 => 5 (38400 baud serial "speed")
    P03.01 => 10 (this is modbus address of VFD)
    P03.02 => 0 (setting data transfer format, this is SERIAL_8O1)
    P07.08 => 5 (VFD frequency set via RS485)
*/

/*
  Here we're creating the object for the library.
  Parameters are:
  VFD inverter(address, serial, baudrate, comm_pin)
    - address: address of the VFD (P03.01). In this example the address of VFD is 10
    - serial: we're using a MEGA board with 3 serials. I'm using Serial2 for RS485 communication here.
    - baudrate: communication speed, setted in parameter P03.00 (in this case is 38400 baud)
    - comm_pin: on MAX485 there are pins DE and RE to set the module in transmit or receive mode.
                in this case we're using just pin D15 to switch between both modes (pin D15 connected to BOTH RE and DE pins on the module)
*/
const int comm_pin = 15;  // using pin D15 for commutating between receive and transmit mode in MAX485
VFD inverter(10, Serial2, 38400, comm_pin);


void setup() {
  Serial.begin(9600); // Start USB serial, so you can print to serial monitor

  // Serial2 is our RS485 serial. The VFD parameter P03.02 sets the data transfert format
  // in this case it's setted to be 8bit data, 1 odd parity check, 1 stop bit
  // i got troubles using 8N1 (default arduino serial data transfer format, value 2 in parameter P03.02)
  // i believe that's due to the device using a 2nd stop bit (see https://modbus.org/docs/Modbus_over_serial_line_V1_02.pdf page 12 sec. 2.5.1)
  Serial2.begin(38400, SERIAL_8O1);

  // now we begin the library
  inverter.begin();


  // Set the frequency to 10Hz
  Serial.println("Setting speed to 10Hz");
  inverter.setSpeed(10.0);
  // check if any error in communication
  // NOTE:
  // in case of setting functions (run(), stop(), setSpeed(), etc...) even if you get wrong CRC or other errors you probably will have the parameter set anyway
  Serial.println(inverter.lastCommError());
  Serial.println(); // empty line for better reading
  delay(500);

  // now let's see if the speed got setted
  Serial.print("Read speed: ");
  Serial.println(inverter.getAimFreq(), 1); // returns float, only 1 decimal stored in VFD
  // check if any error in communication
  // NOTE: in this case if we got any error last returned value is uncorrect
  // You can use function inverter.lastCommErrorNum() to get the error number and see what went wrong.
  Serial.println(inverter.lastCommError());
  Serial.println(); // empty line for better reading
  delay(500);

  // WARNING!! Here comes the running motor!
  Serial.println("START inverter");
  inverter.run();
  delay(2000);

  // Now stopping the motor
  Serial.println("STOP inverter");
  inverter.stop();
  // Depending on your frequency and stopping mode (P00.03) you may need some time to stop the inverter
  // so put a 5 seconds delay waiting for the inverter to full stop.
  delay(5000);

  // WARNING!! Here comes the running motor!
  Serial.println("START Forward direction");
  inverter.runForward();
  delay(2000);

  // Now stopping the motor
  Serial.println("STOP inverter");
  inverter.stop();
  delay(5000);

  // WARNING!! Here comes the running motor!
  Serial.println("START Backward direction");
  inverter.runBackward();
  delay(2000);

  // Now stopping the motor
  Serial.println("STOP inverter");
  inverter.stop();
  delay(5000);

  // end of the test code, loop() is empty
  Serial.println();
  Serial.println();
  Serial.println("End of test");

}

void loop() {
  // nothing to do here

}
