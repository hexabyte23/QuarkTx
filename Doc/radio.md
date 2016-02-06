# Radio user guide

## First compilation

You must first install the arduino software (IDE) from the official arduino site <http://www.arduino.cc> in download section. Then, get a copy of QuarkTx source code from <https://github.com/hexabyte23/QuarkTx> by using `Download ZIP` button or by using [GitHub desktop](https://desktop.github.com).

Once the source code is put in your local PC/Mac, doubleclick on Radio/Radio.ino file. Arduino software will automatically be opened, and Radio.ino source file displayed.

>For **Arduino Nano** user
>
>Before compiling code, you must first select **Arduino Nano** in the Tools/Board type menu. 

or
  
>For **Teensy** user
>
>Before compiling code, you must first install [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html), then select **Teensy 3.2/3.1** in the Tools/Board type menu. 

Once this selection is done, you can plug your device to your local PC/Mac, and press `upload` button.

## First start

Upload is now complete, open Serial Monitor from Arduino IDE, and  you see below first messages during booting sequence:

	Quark Tx v0.1.0
	Booting...	Serial		OK
	Command		OK
	e-edic
	Tx		OK
	2004 µs
	Nano
	Ready
	>

The booting section show a line started with **e-**, that means an error is raised during booting. (See [**Error messages**](#error-pane) pane for errors detail).

This '**e-edic**' error can occurs in 2 cases : You really have an EEPROM data corruption or this is the first time ever you start QuartTx. To clean this error we must calibrate all sensors connected.
 

### First calibration

Sensors calibration would  setup limits of all sensors curces. to do so, make sure first that all sensors are connected to the board (as discribe on electric diagram), then execute toggle command `c`. This command make you enter in the calibration mode and you must execute `c` command again to exit from this mode.

In calibration mode you will see real time datas moving like that:

	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	

Each pair of numeric data inside {} are the minimal and the maximum limit value (range 0 to 1023) of a given sensor. In that example, sensor 0 minimum limit is 381 and maximum is 627, sensor 1 minimum limit is 393 and maximum is 631, and so on ... 

To calibrate sensors, you must move all sensors physically from their minimum curce to their maximum curce until min/max of each sensors become stable to a unique value. (do not forget mechanical trims on old gimbals and mechanical switches).

***Warning***: Make sure to reach sensor real limits to prevent
 any limit boundary overflow during your fligth that should cause a crash !

When its done, you can exit calibrate mode (by execute `c` again). 

Its now time to save to EEPROM your settings by executing `v` command. You can check that operation succeed to press the hardware reset button on the board. The e-edic error is now supposed to disappear during booting sequence otherwise you have a real EEPROM data corruption problem.

You can now use one of the 22 commands (for v 0.1.0) available (check [**Commands**](#commands-pane) pane for details) or use Gui app(still in dev step) for Win 10/OSX/Android or IOS.

Have fun.

## <a name"commands-pane"></a> Commands

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
* w: Toggle simulation mode

## <a name"error-pane"></a> Error messages
* e-bp Bad parameter
	* You are using a bad parameter in the current command 
* e-edic EEPROM data is corrupted
	* Self-explanatory
* e-sdf Switch in debug mode first
	* You cannot switch to simulation mode before entering first un debug mode
* e-pf RCL parsing failed
	* Usually syntax error in RCL formula
* e-bcf Bad command format
* e-cu Command unknown

## <a name"sensors-pane"></a> Sensors
Sensors group all physical device (Gimbal, switch, button) you can add in the RC command.

### Gimbal
To be define

### Switch
To be define

## <a name"rf-module-pane"></a> RF module
To be define

## <a name"rcl-pane"></a> Radio Control Language
This new language aim is to describe all sensors dependencies for each channels.

### Sensor variable
To be define

### Dual rate function
To be define
 
### Reverse function
To be define

### Exponential function
To be define

### Mixer
To be define

### Complex functions
