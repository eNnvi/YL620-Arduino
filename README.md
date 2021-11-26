# YL620 control library for arduino
___

## Todo list

- [x] Initial release
- [ ] Make sure every method works as expected
- [ ] Create some examples
- [ ] Publish schematics and few pics/video as proof it's working
- [ ] Write better documentation (hard task but somebody has to do it)

---

## Description

This library allows you to control a YL620 VFD (Variable frequency drive) with an Arduino connected via Modbus control.

At this stage most of the main VFD functions are available (like start, stop, set speed, read speed, acceleration and deceleration, parameters set, ecc...).

## This library is still under development
### Lot of testing is still needed, use at your own risk!


___


## Connections
Commands are sent via RS485. You need a RS232 to RS485 converter for arduino.
I'm using a MAX485 based conversion circuit, connecting RE and DE pins togheter to an arduino pin. These pins are used to switch communication mode between "receive" and "transmit" mode.

If you use full duplex adapter no comm_pin is needed and the library will handle that.

___

## Parameters setup
When controlling the VFD via RS485 you need to set the VFD to listen via RS485, accept input and set modbus address.

I got successful communication with following parameters:
- P00.01 => 3 (allows command over RS485)
- P03.00 => 5 (38400 baud serial "speed")
- P03.01 => 10 (this is modbus address of VFD, just set it according in library)
- P03.02 => 0 (setting data transfer format, this is SERIAL_8O1. Set it according in your `Serial.begin(baud, mode)`)
- P07.08 => 5 (VFD frequency set via RS485)

whit these parameter an easy implementation is:
```
static int comm_switch_pin = 15; // Arduino pin connected to RE and DE pins on the MAX485


/*
 * set up library with
 * VFD address 10
 * using Serial2 interface (on a Mega board here, you can also use SoftwareSerial)
 * 38400 baud
 * using pin 15 for commutating transmit/receive mode
*/
VFD inverter(10, Serial2, 38400, comm_switch_pin);

void setup() {

  ...
  
  // Setup serial interface with correct baud and data format
  // in this case 8 bit, 1 stop, odd parity
  Serial2.begin(38400, SERIAL_8O1);
  inverter.begin();
  
  ...
  
}
```

then check the [API](https://github.com/eNnvi/YL620-Arduino/doc/api.md) for full documentation