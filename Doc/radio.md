# Radio documentation

## First compilation

You must first install the arduino software (IDE) from the official arduino site <http://www.arduino.cc> in download section. then, get a copy of QuarkTx source code from <https://github.com/hexabyte23/QuarkTx> by using `Download ZIP` button or by using [GitHub desktop](https://desktop.github.com).

Once the source code put in your local PC/Mac, you can then doubleclick on Radio/Radio.ino file. Arduino software will be automatically opened, and Radio.ino source file will be displayed.

>For **Arduino Nano** user
>
>Before compiling code, you must first select **Arduino Nano** in the Tools/Board type menu. 

or
  
>For **Teensy** user
>
>Before compiling code, you must first install [Teensyduino](https://www.pjrc.com/teensy/teensyduino.html) then
>select **Teensy 3.2/3.1** in the Tools/Board type menu. 


You can then, plug your device to your local PC/Mac, and press `upload` button.

## First start

Once the upload is complete, you can then open Serial Monitor from Arduino IDE, see below the first booting messages:

	Quark Tx v0.1.0
	Booting...	Serial		OK
	Command		OK
	e-edic
	Tx		OK
	2004 µs
	Nano
	Ready
	>

The booting section show a line started with **e-**, that means an error is raised. (See [**Error messages**](#error-pane) pane for details).

In our case **e-edic** means EEPROM data is corrupted. It's quite regular as we never write setting in EEPROM memory.

The first command you must execute is sensors calibration, to setup limits of all sensors curces. to do so, make sure first that all sensors are connected to the board, then execute toggle command `c`. This command make you enter in the calibration mode and you must execute `c` command again to exit from this mode.

In calibration mode you will see

	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	
	{381	627}	{393	631}	{389	635}	{385	627}	{0	1}	{0	1}	{0	1}	

Each pair of data inside {} are the minimal and the maximum limit value of a given sensor. In that example, sensor 0 minimum limit is 381 and maximum is 627, sensor 1 minimum limit is 393 and maximum is 631, and so on ... 

To calibrate sensors, you must move all sensors physically from minimum curce to maximum curce until min/max of each sensors become stable to a unique value. (do not forget mechanical trims on old gimbals and mechanical switches)

22 commands (for v 0.1.0) are available (check [**Commands**](#commands-pane) pane for details). 

### <a name"commands-pane"></a> Commands

All commands in QuarkTx are only one character long. They can  have some optionnal setting marked by [].


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
* e-edic		EEPROM data is corrupted
* e-sdf		Switch in debug mode first
* e-pf		RCL parsing failed
* e-bcf		Bad command format
* e-cu		Command unknown

## Sensors

## Channels
