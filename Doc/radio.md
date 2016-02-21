# Radio user guide

## First compilation

You should first install the arduino software (IDE) from the official arduino site <http://www.arduino.cc> in download section. Then, get a copy of QuarkTx source code from <https://github.com/hexabyte23/QuarkTx> by using `Download ZIP` button or by using [GitHub desktop](https://desktop.github.com).

Once the source code is put in your local PC/Mac, doubleclick on Radio/Radio.ino file. Arduino software will automatically be opened, and Radio.ino source file displayed.

>For **Arduino Nano** user
>
>Before compiling code, you must first select **Arduino Nano** in the Tools/Board type menu. 

or
  
>For **Teensy** user
>
>Before compiling code, you must first install [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html), then select **Teensy 3.2/3.1** in the Tools/Board type menu. 

Once this selection is done, you can plug your device to your local PC/Mac, and hit `upload` button.

## First start

Upload is now complete, open Serial Monitor from Arduino IDE. The booting sequence messages is as below:

	Quark Tx v0.1.0
	Booting...	Serial		OK
	Command		OK
	e-edic
	Tx		OK
	2004 µs
	Nano
	Ready
	>

The line started with **e-** is an error raised during booting sequence. (See [Error messages](#error-messages) pane for errors detail).

This specific '**e-edic**' error can occurs in 2 cases : You really have an EEPROM data corruption problem or this is the first time ever you start QuartTx. To clean this error you must calibrate all sensors connected, then save the result.
 

### First calibration

Sensors calibration would setup limits of all sensors curses. to do so, make sure first that all sensors are connected to the board (as describe on electric diagram), then execute toggle command `c`. This command make you enter in the calibration mode. You must execute `c` command again to exit from this mode.

In calibration mode you will see real time datas moving like that:

	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	

Each pair of numeric data inside {} are the minimal and the maximum limit value (range 0 to 1023) for a given sensor. In that example, sensor 0 minimum limit is 381 and maximum is 627, sensor 1 minimum limit is 393 and maximum is 631, and so on ... 

To calibrate sensors, you must move all sensors physically from their minimum curse to their maximum curse until min/max of each sensors become stable to a unique value. (do not forget mechanical trims on old gimbals and mechanical switches).

***Warning***: Make sure to reach sensor real limits to prevent
 any limit boundary overflow during your fligth that should cause a crash !

When its done, you can exit calibrate mode (`c`). 

Its now time to save to EEPROM your settings by executing `v` command. You can check if that operation succeed by hiting the hardware reset button on the board. The **e-edic** error is now supposed to disappear during booting sequence otherwise you have a real EEPROM data corruption problem.

You can now use one of the 22 commands (for v 0.1.0) available (check [Commands](#commands) for details) or use Gui app (still in dev) for Win 10/OSX/Android or IOS.

Have fun.

## Commands

All commands in QuarkTx are only one character long. Optionnal setting marked by **[]**, alternate option by **|**.


* a: Load from EEPROM
* c: Toggle sensors calibration
* d [m]|[e]|[s]|[l]: Dump model, EEPROM, sensor or RCL
* f: Get free memory
* h: Help
* i [freq]: Toggle input sensor update
* l (0..1): Load cur. model
* m: Toggle Tx mode (transmit or debug)
* n: Display version
* o [freq]: Toggle output PPM update
* r [m]|[s]|[l]: Clean model, sensor or RCL
* s a chan val: Set cur. model servo max
* s i chan val: Set cur. model servo min
* s l chan rclStr: Set RCL code to PPM chan
* s n chan val: Set cur. model servo neutral
* s r chan val: Set cur. model servo revert
* s t sensorID val: Set sensor trim
* s u sensorID val: Set simulate value
* s v sensorID val: Set sensor Min
* s w sensorID val: Set sensor Max
* v: Save to EEPROM

## Error messages
All serial message starting with **e-** is an error message code. It's not directly human readable but have a very short printfoot in memory (For Arduino board).

Error msg | Short description| Long description
----------|------------------|-----------------
e-bp | Bad parameter | You are using a bad parameter in the current command
e-bpim | Bad parameter: index is missing |
e-edic | EEPROM data is corrupted | Self-explanatory
e-pf | RCL parsing failed | Usually syntax error in RCL formula
e-bcf | Bad command format |
e-cu | Command unknown |
e-nt | None type | Bad RCL nodes initialization
e-cstl | Command string too long


## <a name"sensors-pane"></a> Hardware

### Arduino nano schema
![schema](nano_sch.png)

### Teensy schema
![schema](teensy_sch.png)

### Arduino board
To be define

### Teensy board
![schema](teensy_board.png)

### Sensors
Sensors are all physical device (Gimbal, switch, button) that you can add in QuarkTx board.

#### Gimbal
To be define

#### Switch
To be define

## <a name"rf-module-pane"></a> RF module
QuarkTx is able to generate PPM signal for different type of RF module.

### Tested modules

Company | Name | # channels | Inter frame | Inter channel
--------|------|------------|-------------|--------------
Jeti | TU2 2.4 Ghz | tested, up to 17 | 4500 | 1500
FrSky | DHT 8ch DIY | tested, up to 4 | 4500 | 1500 

### Output PPM signal
To fine tune PPM signal, depending of the RF module you use, you can modify the 2 constants below (config.h):

* `PPM_INTER_CHANNEL_TIME` 
* `PPM_INTER_FRAME_TIME`

![PPM](PPM.png)


Exemple of Arduino scope screen copy with 4 gimbals/3 switches/6 channels hardware config


![scope ardiono](ppm-scope-arduino-6c.png)



Exemple of Teensy scope screen copy with 4 gimbals/3 switches/6 channels hardware config



![scope ardiono](ppm-scope-teensy-6c.png)



Exemple of Teensy scope screen copy with 4 gimbals/3 switches/24 channels hardware config
![scope ardiono](ppm-scope-teensy-24c.png)

## <a name"config-pane"></a> Configuration
All constant parameters is group in a single config.h file.

Name | Description 
--------|------
`QUARKTX_SERIAL_SPEED`| to be define
`MAX_MODEL`| to be define
`MAX_SERIAL_INPUT_BUFFER`| to be define
`ADC_MIN_VALUE`| to be define
`ADC_MAX_VALUE`| to be define
`PPM_MIN_VALUE`| See [Output PPM signal](#output-ppm-signal)
`PPM_MAX_VALUE`| See [Output PPM signal](#output-ppm-signal)
`TRIM_MIN_VALUE`| to be define
`TRIM_MAX_VALUE`| to be define
`TX_MODE`| to be define
`LED_BLINK_PERIOD`| to be define
`BATTERY_RATE_UPDATE`| to be define
`BATTERY_HISTO_BUFFER_SIZE`| to be define
`BATTERY_R1`| to be define
`BATTERY_R2`| to be define

## <a name"rcl-pane"></a> Radio Control Language
This new script language has been designed to be able to describe all dependencies between input sensors and PPM output channels. To simplify dependency graph, there is only one script per PPM output channel. Every script is evaluated in real time. Up to 300 update/sec on Arduino Nano board and up to 900 update/sec on Teensy 3.2 board for a simple script.

_Exemple_: `s l chan rclStr` will modify the current RCL script of a given output PPM channel `chan`  with the new script string `rclStr`. A script could not be longer than **`MAX_SERIAL_INPUT_BUFFER`** characters (config.h)

***Warning***: No white space is allowed between identifier/operator/modifier.


### Sensor variable
A sensor variable is define by a single character `i` followed by the sensor index (with index range [O, **`MAX_INPUT_CHANNEL `**]). For exemple `i2` is used for the _third_ sensor, as index started from 0.

By default, the minimal script below is present on every PPM channels during the first booting sequence:  `i*x*` (with x range [0, **`MAX_INPUT_CHANNEL`**]). This is the smalest script you can write to associate one single input sensor to an output channel.

Every time you use reset command `r`, this minimal script will be set on all channels.

The sensor output range of value is [0, 1023].

_Exemple_: `s l 1 i3`. This command will associate sensor 3 values with PPM output 1 values in real time.


### Constant expression
Constant types are : Integer (ex: 1023), float (ex: 0.123), boolean (ex: T or F)

_Exemple1_: `s l 3 10+i2*1.3`


_Exemple2_: `s l 1 F?i1:i2`

### Basic arithmetic operator

Operator:


*expression1***`+`***expression2*


*expression1***`-`***expession2*


*expression1***`*`***expression2*


*expression1***`/`***expression2*



_Exemple_: `s l 0 (i3+1)/3.3-22`

### Sub expression
Command:


**(** sub expression**)**



_Exemple_:  `s l 0 10+(i2/3.3)`

### Dual rate function
Operator [] is used to modify the output range of a sensor variable or a sub expression.

_Exemple 1_: `s l 0 i1[20;30]`. This will reduce the sensor 1 range from [0, 1023] to [20, 30].

_Exemple2_: `s l 0 (i1+10)[100;500]`. More complex script to reduce the range of a sub expression result.

### Reverse function
Operator [] is used to reverse the output range of a sensor variable or a sub expression.

_Exemple_: `s l 0 i1[1023;0]`. This will change the output value of sensor 1 range from [0, 1023] to [1023, 0], so the signal will be reversed.

### Exponential function
To be define

### Mixer function
To be define


### IF function

Command: *expression***?***action1***:***action2*

_Exemple_: `(i1>512)?i2:i2[1023:0]` will return i2 until i1 < 512 then reverse i2