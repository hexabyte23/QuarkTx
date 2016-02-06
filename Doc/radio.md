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



### Commands

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

## Error messages

## Sensors

## Channels
