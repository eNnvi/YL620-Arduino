# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`VFD`](#class_v_f_d) | [VFD](#class_v_f_d) class for inverter control.

# class `VFD` 

[VFD](#class_v_f_d) class for inverter control.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`VFD`](#class_v_f_d_1aa8a03e4ff89a53b7932ad859eb612ef0)`(uint8_t _address,Stream & _comm_stream)` | Constructor.
`public  `[`VFD`](#class_v_f_d_1a101a3418dbfa512d132ceb8df2870222)`(uint8_t _address,Stream & _comm_stream,int baud)` | Constructor.
`public  `[`VFD`](#class_v_f_d_1a5d981a13261ce3b9e76c2327ccb3b009)`(uint8_t _address,Stream & _comm_stream,int baud,uint8_t _comm_pin)` | Constructor.
`public void `[`begin`](#class_v_f_d_1a7c5c4a282c82adf3ae56b9379105b021)`()` | First call for pin settings.
`public void `[`setSpeed`](#class_v_f_d_1a685050d263bdffe2df09f20f756a318e)`(float speed)` | Sets frequency on the [VFD](#class_v_f_d).
`public void `[`run`](#class_v_f_d_1a94b6777ba5b01f9ffce6b850fcc2f2f3)`()` | Starts the motor.
`public void `[`stop`](#class_v_f_d_1a236ea9b315cb4c733529317e25480afb)`()` | Stops the motor.
`public void `[`runForward`](#class_v_f_d_1a569f7b8766b2486832a0fa2a0e34e0e7)`()` | STARTS THE MOTOR setting forward rotation.
`public void `[`runBackward`](#class_v_f_d_1a3ee396f7b66b30fdf70d37c4d9f94b35)`()` | STARTS THE MOTOR setting backward rotation.
`public void `[`changeDirection`](#class_v_f_d_1addc6300a356687e963e2b59ad1de2631)`()` | changes direction of the motor (no matter if running or not ?)
`public void `[`setForward`](#class_v_f_d_1ad2227e20e924e7a7863d3317faca1aad)`()` | changes direction of the motor to forward (no matter if running or not ?)
`public void `[`setBackward`](#class_v_f_d_1ad5dc9aa34e5ae8e5cbde94391ab7d6cc)`()` | changes direction of the motor to backward (no matter if running or not ?)
`public void `[`resetError`](#class_v_f_d_1a076d315731acf881c41c0b60979b2b82)`()` | resets last [VFD](#class_v_f_d) error
`public void `[`resetAllErrors`](#class_v_f_d_1a4c5463a9cbb6b7702a64dd9011a1eed4)`()` | resets all [VFD](#class_v_f_d) error
`public `[`VFD_Errors`](#vfd_8h_1ae3913d24e126eca942cd7007eade9654)` `[`getError`](#class_v_f_d_1a039fadad070e87c4c0b3e5f8c6113e4f)`()` | returns the [VFD](#class_v_f_d) error
`public float `[`getRunFreq`](#class_v_f_d_1a9bb628dccf7673e6f79dfa2f43d464a0)`()` | get actual running frequency
`public uint16_t `[`getCpuID`](#class_v_f_d_1affb69b90f484a72ae9adb18916fdec8b)`()` | get Cpu unique id of the [VFD](#class_v_f_d)
`public float `[`getAimFreq`](#class_v_f_d_1a5919529c4d66f7db381437aa2e7cc7cb)`()` | get target running frequency
`public float `[`getOutCurrent`](#class_v_f_d_1acbdb8d06b2c7fa9d1bc73fe43016f4d6)`()` | get actual output current
`public float `[`getRunVoltage`](#class_v_f_d_1ad83dac8bc5e1398e7d762242fe0c7598)`()` | get actual running voltage
`public float `[`getBusVoltage`](#class_v_f_d_1a83b253aae88d1d898b26807a35e785b2)`()` | Get actual bus voltage.
`public float `[`getAccelTime`](#class_v_f_d_1a718e8b568c400849cc94db1648d7b9a0)`()` | Get actual acceleration time.
`public void `[`setAccelTime`](#class_v_f_d_1aca2f19a81d1073b9796f0dd5379d761f)`(float time)` | Sets acceleration time.
`public float `[`getDecelTime`](#class_v_f_d_1a8b890ed8e80ef098083f0a292d4766cd)`()` | Get actual deceleration time.
`public void `[`setDecelTime`](#class_v_f_d_1af8e1803ee4accbba46a6a5456a202988)`(float time)` | Sets deceleration time.
`public char * `[`lastCommError`](#class_v_f_d_1ab89a8840263e507ca40108cda6edb846)`()` | Get last library (communication) error.
`public int `[`status`](#class_v_f_d_1a8f0bfeb64b7016f5039d61b2e91362f1)`()` | Gets if inverter is running.
`public bool `[`isForward`](#class_v_f_d_1a025aec6902a4ddb344bd8ee2be42057e)`()` | Gets if inverter is in forward direction.
`public bool `[`isBackward`](#class_v_f_d_1a8c831b0bad9c22ab5a064cbc0c440723)`()` | Gets if inverter is in backward direction.
`public uint16_t `[`getParameter`](#class_v_f_d_1a5c4962544bc44f0629de7059a045fa98)`(uint8_t section,uint8_t param)` | Get a parameter of the inverter.
`public `[`VFD_Comm_Errors`](#vfd_8h_1af63f127086426f531de4351494f526d3)` `[`setParameter`](#class_v_f_d_1afd9e98bfe3a25faa8d67f9be673685ac)`(uint8_t section,uint8_t param,uint16_t value)` | Set a parameter of the inverter.
`public `[`VFD_Comm_Errors`](#vfd_8h_1af63f127086426f531de4351494f526d3)` `[`update`](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)`()` | Retrieves all the running parameter of the inverter.
`public float `[`fetchAccelTime`](#class_v_f_d_1a2ecfc52b9a25813091ad9f17012cbe29)`()` | Retrieves Acceleration time from library.
`public float `[`fetchDecelTime`](#class_v_f_d_1a3c29bb8ce398ede82f059336971d99e8)`()` | Retrieves Deceleration time from library.
`public float `[`fetchAimFrequency`](#class_v_f_d_1a039e16341fc1714e6e0acd06f5a7ffc8)`()` | Retrieves aim frequency from library.
`public float `[`fetchRunFrequency`](#class_v_f_d_1a42a089fa06de34935b3b7e4ee8dd2a64)`()` | Retrieves run frequency from library.
`public uint16_t `[`fetchOutCurrent`](#class_v_f_d_1a1aa4e3357b28fad952cf39f28cb1b9b5)`()` | Retrieves output current from library.
`public uint16_t `[`fetchRunVoltage`](#class_v_f_d_1a7b4850d1209afccba55a821a65758b9f)`()` | Retrieves running voltage from library.
`public uint16_t `[`fetchBusVoltage`](#class_v_f_d_1aa3a31f868809b0a02f736a03becd07b0)`()` | Retrieves bus voltage from library.
`public uint16_t `[`fetchOperatingCommand`](#class_v_f_d_1a69087c611033a32c0c09060f63e85518)`()` | Retrieves command register value.
`public uint16_t `[`fetchCPUId`](#class_v_f_d_1adb328337726d2d4f369d677b04844c58)`()` | Retrieves unique id.
`public bool `[`fetchDirection`](#class_v_f_d_1a1f47640b9318ed3b4d1ccb7a652b37fa)`()` | Retrieves direction of the [VFD](#class_v_f_d).
`public bool `[`fetchForward`](#class_v_f_d_1a81d57296e79bb217e07c158a2dc4e1d9)`()` | Retrieves if the [VFD](#class_v_f_d) is running forward.
`public bool `[`fetchBackward`](#class_v_f_d_1afb69ba7c0b4d0a25b64ffb09c27379f1)`()` | Retrieves if the [VFD](#class_v_f_d) is running backward.
`public bool `[`fetchRunning`](#class_v_f_d_1afea9752606f4f44d4e429b522527b351)`()` | Retrieves if the [VFD](#class_v_f_d) is running.

## Members

#### `public  `[`VFD`](#class_v_f_d_1aa8a03e4ff89a53b7932ad859eb612ef0)`(uint8_t _address,Stream & _comm_stream)` 

Constructor.

Create a new [VFD](#class_v_f_d) object. Required params address and comm_stream. If no baud specified used 9600. If no comm_pin specified doesn't switch during comm (use with full-duplex adapter). 
#### Parameters
* `_address` address of the [VFD](#class_v_f_d) (param P03.01). 

* `_comm_stream` communication stream (Serial1, Serial, VirtualSerial, ecc...). 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream, int baud)](#class_v_f_d_1a101a3418dbfa512d132ceb8df2870222); 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream, int baud, uint8_t _comm_pin)](#class_v_f_d_1a5d981a13261ce3b9e76c2327ccb3b009);

#### `public  `[`VFD`](#class_v_f_d_1a101a3418dbfa512d132ceb8df2870222)`(uint8_t _address,Stream & _comm_stream,int baud)` 

Constructor.

Create a new [VFD](#class_v_f_d) object. Required params address and comm_stream. If no baud specified used 9600. If no comm_pin specified doesn't switch during comm (use with full-duplex adapter). 
#### Parameters
* `_address` address of the [VFD](#class_v_f_d) (param P03.01). 

* `_comm_stream` communication stream (Serial1, Serial, VirtualSerial, ecc...). 

* `baud` [VFD](#class_v_f_d) communication baudrate (param P03.00). 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream)](#class_v_f_d_1aa8a03e4ff89a53b7932ad859eb612ef0); 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream, int baud, uint8_t _comm_pin)](#class_v_f_d_1a5d981a13261ce3b9e76c2327ccb3b009);

#### `public  `[`VFD`](#class_v_f_d_1a5d981a13261ce3b9e76c2327ccb3b009)`(uint8_t _address,Stream & _comm_stream,int baud,uint8_t _comm_pin)` 

Constructor.

Create a new [VFD](#class_v_f_d) object. Required params address and comm_stream. If no baud specified used 9600. If no comm_pin specified doesn't switch during comm (use with full-duplex adapter). 
#### Parameters
* `_address` address of the [VFD](#class_v_f_d) (param P03.01). 

* `_comm_stream` communication stream (Serial1, Serial, VirtualSerial, ecc...). 

* `baud` [VFD](#class_v_f_d) communication baudrate (param P03.00). 

* `_comm_pin` Arduino pin for commutating trasmit/receive mode. 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream)](#class_v_f_d_1aa8a03e4ff89a53b7932ad859eb612ef0); 

**See also**: [VFD(uint8_t _address, Stream &_comm_stream, int baud)](#class_v_f_d_1a101a3418dbfa512d132ceb8df2870222);

#### `public void `[`begin`](#class_v_f_d_1a7c5c4a282c82adf3ae56b9379105b021)`()` 

First call for pin settings.

#### `public void `[`setSpeed`](#class_v_f_d_1a685050d263bdffe2df09f20f756a318e)`(float speed)` 

Sets frequency on the [VFD](#class_v_f_d).

#### Parameters
* `speed` float of speed (max 1 decimal unit)

#### `public void `[`run`](#class_v_f_d_1a94b6777ba5b01f9ffce6b850fcc2f2f3)`()` 

Starts the motor.

#### `public void `[`stop`](#class_v_f_d_1a236ea9b315cb4c733529317e25480afb)`()` 

Stops the motor.

#### `public void `[`runForward`](#class_v_f_d_1a569f7b8766b2486832a0fa2a0e34e0e7)`()` 

STARTS THE MOTOR setting forward rotation.

#### `public void `[`runBackward`](#class_v_f_d_1a3ee396f7b66b30fdf70d37c4d9f94b35)`()` 

STARTS THE MOTOR setting backward rotation.

#### `public void `[`changeDirection`](#class_v_f_d_1addc6300a356687e963e2b59ad1de2631)`()` 

changes direction of the motor (no matter if running or not ?)

#### `public void `[`setForward`](#class_v_f_d_1ad2227e20e924e7a7863d3317faca1aad)`()` 

changes direction of the motor to forward (no matter if running or not ?)

#### `public void `[`setBackward`](#class_v_f_d_1ad5dc9aa34e5ae8e5cbde94391ab7d6cc)`()` 

changes direction of the motor to backward (no matter if running or not ?)

#### `public void `[`resetError`](#class_v_f_d_1a076d315731acf881c41c0b60979b2b82)`()` 

resets last [VFD](#class_v_f_d) error

#### `public void `[`resetAllErrors`](#class_v_f_d_1a4c5463a9cbb6b7702a64dd9011a1eed4)`()` 

resets all [VFD](#class_v_f_d) error

#### `public `[`VFD_Errors`](#vfd_8h_1ae3913d24e126eca942cd7007eade9654)` `[`getError`](#class_v_f_d_1a039fadad070e87c4c0b3e5f8c6113e4f)`()` 

returns the [VFD](#class_v_f_d) error

#### Returns
code of the error

#### `public float `[`getRunFreq`](#class_v_f_d_1a9bb628dccf7673e6f79dfa2f43d464a0)`()` 

get actual running frequency

#### Returns
running frequency in Hz

#### `public uint16_t `[`getCpuID`](#class_v_f_d_1affb69b90f484a72ae9adb18916fdec8b)`()` 

get Cpu unique id of the [VFD](#class_v_f_d)

#### Returns
raw cpuid data

#### `public float `[`getAimFreq`](#class_v_f_d_1a5919529c4d66f7db381437aa2e7cc7cb)`()` 

get target running frequency

#### Returns
running frequency in Hz

#### `public float `[`getOutCurrent`](#class_v_f_d_1acbdb8d06b2c7fa9d1bc73fe43016f4d6)`()` 

get actual output current

#### Returns
out current in Amperes

#### `public float `[`getRunVoltage`](#class_v_f_d_1ad83dac8bc5e1398e7d762242fe0c7598)`()` 

get actual running voltage

#### Returns
out voltage in Volts

#### `public float `[`getBusVoltage`](#class_v_f_d_1a83b253aae88d1d898b26807a35e785b2)`()` 

Get actual bus voltage.

#### Returns
Bus voltage in Volts

#### `public float `[`getAccelTime`](#class_v_f_d_1a718e8b568c400849cc94db1648d7b9a0)`()` 

Get actual acceleration time.

#### Returns
Acceleration time in ms

#### `public void `[`setAccelTime`](#class_v_f_d_1aca2f19a81d1073b9796f0dd5379d761f)`(float time)` 

Sets acceleration time.

#### Parameters
* `time` Acceleration time in ms

#### `public float `[`getDecelTime`](#class_v_f_d_1a8b890ed8e80ef098083f0a292d4766cd)`()` 

Get actual deceleration time.

#### Returns
Deceleration time in ms

#### `public void `[`setDecelTime`](#class_v_f_d_1af8e1803ee4accbba46a6a5456a202988)`(float time)` 

Sets deceleration time.

#### Parameters
* `time` Deceleration time in ms

#### `public char * `[`lastCommError`](#class_v_f_d_1ab89a8840263e507ca40108cda6edb846)`()` 

Get last library (communication) error.

#### Returns
Human readable last error (C-style string)

#### `public int `[`status`](#class_v_f_d_1a8f0bfeb64b7016f5039d61b2e91362f1)`()` 

Gets if inverter is running.

#### Returns
0 if stopped, 1 if running, 2 if accelerating/decelerating

#### `public bool `[`isForward`](#class_v_f_d_1a025aec6902a4ddb344bd8ee2be42057e)`()` 

Gets if inverter is in forward direction.

#### Returns
true if forward, false otherwise

#### `public bool `[`isBackward`](#class_v_f_d_1a8c831b0bad9c22ab5a064cbc0c440723)`()` 

Gets if inverter is in backward direction.

#### Returns
true if backward, false otherwise

#### `public uint16_t `[`getParameter`](#class_v_f_d_1a5c4962544bc44f0629de7059a045fa98)`(uint8_t section,uint8_t param)` 

Get a parameter of the inverter.

#### Parameters
* `section` parameter section (es. P01.05 section is 01) 

* `param` parameter (es. P01.05 param is 05) 

#### Returns
value of the stored register

#### `public `[`VFD_Comm_Errors`](#vfd_8h_1af63f127086426f531de4351494f526d3)` `[`setParameter`](#class_v_f_d_1afd9e98bfe3a25faa8d67f9be673685ac)`(uint8_t section,uint8_t param,uint16_t value)` 

Set a parameter of the inverter.

#### Parameters
* `section` parameter section (es. P01.05 section is 01) 

* `param` parameter (es. P01.05 param is 05) 

* `value` value to set 

#### Returns
communication error enum

#### `public `[`VFD_Comm_Errors`](#vfd_8h_1af63f127086426f531de4351494f526d3)` `[`update`](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)`()` 

Retrieves all the running parameter of the inverter.

#### Returns
communication error enum

#### `public float `[`fetchAccelTime`](#class_v_f_d_1a2ecfc52b9a25813091ad9f17012cbe29)`()` 

Retrieves Acceleration time from library.

#### Returns
Acceleration time in ms 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public float `[`fetchDecelTime`](#class_v_f_d_1a3c29bb8ce398ede82f059336971d99e8)`()` 

Retrieves Deceleration time from library.

#### Returns
Deceleration time in ms 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public float `[`fetchAimFrequency`](#class_v_f_d_1a039e16341fc1714e6e0acd06f5a7ffc8)`()` 

Retrieves aim frequency from library.

#### Returns
Frequency vfd is trying to reach 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public float `[`fetchRunFrequency`](#class_v_f_d_1a42a089fa06de34935b3b7e4ee8dd2a64)`()` 

Retrieves run frequency from library.

#### Returns
Frequency vfd is running at 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public uint16_t `[`fetchOutCurrent`](#class_v_f_d_1a1aa4e3357b28fad952cf39f28cb1b9b5)`()` 

Retrieves output current from library.

#### Returns
Output current 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public uint16_t `[`fetchRunVoltage`](#class_v_f_d_1a7b4850d1209afccba55a821a65758b9f)`()` 

Retrieves running voltage from library.

#### Returns
Output voltage 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public uint16_t `[`fetchBusVoltage`](#class_v_f_d_1aa3a31f868809b0a02f736a03becd07b0)`()` 

Retrieves bus voltage from library.

#### Returns
Bus voltage 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public uint16_t `[`fetchOperatingCommand`](#class_v_f_d_1a69087c611033a32c0c09060f63e85518)`()` 

Retrieves command register value.

Fetching operating command register status. Need to see if useful after testing or remove it... 
#### Returns
command register value 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public uint16_t `[`fetchCPUId`](#class_v_f_d_1adb328337726d2d4f369d677b04844c58)`()` 

Retrieves unique id.

#### Returns
Raw cpu id 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public bool `[`fetchDirection`](#class_v_f_d_1a1f47640b9318ed3b4d1ccb7a652b37fa)`()` 

Retrieves direction of the [VFD](#class_v_f_d).

#### Returns
TRUE if running forward - FALSE if running backward 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public bool `[`fetchForward`](#class_v_f_d_1a81d57296e79bb217e07c158a2dc4e1d9)`()` 

Retrieves if the [VFD](#class_v_f_d) is running forward.

#### Returns
TRUE if running forward - FALSE if not 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public bool `[`fetchBackward`](#class_v_f_d_1afb69ba7c0b4d0a25b64ffb09c27379f1)`()` 

Retrieves if the [VFD](#class_v_f_d) is running backward.

#### Returns
TRUE if running backward - FALSE if not 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

#### `public bool `[`fetchRunning`](#class_v_f_d_1afea9752606f4f44d4e429b522527b351)`()` 

Retrieves if the [VFD](#class_v_f_d) is running.

#### Returns
TRUE if running - FALSE if stopped 

**See also**: [update()](#class_v_f_d_1aa68aa1e7624c74748adb0c606d8901f2)

Generated by [Moxygen](https://sourcey.com/moxygen)