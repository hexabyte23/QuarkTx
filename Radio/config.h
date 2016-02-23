/*
Config.h - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef CONFIG_H
#define CONFIG_H

// General definitions

#define QUARKTX_VERSION           "0.1.2"
#define QUARKTX_SERIAL_SPEED      9600

#define MAX_MODEL                 2
#define MAX_SERIAL_INPUT_BUFFER   40      // warning: dont set too small value for RCL commands

#define ADC_MIN_VALUE             0
#define ADC_MAX_VALUE             1023

#define PPM_MIN_VALUE             500
#define PPM_MAX_VALUE             1500

#define TRIM_MIN_VALUE            -100
#define TRIM_MAX_VALUE            100

#define TX_MODE                   1       // mode 1 : left stick operates elevator & rudder, right stick operates throttle & ailerons
                                          // mode 2 : left stick operates throttle & rudder, right stick operates elevator & ailerons
                                          // mode 3 : left stick operates elevator & ailerons, right stick operates throttle & rudder
                                          // mode 4 : left stick operates throttle & ailerons, right stick operates elevator & rudder

#define LED_BLINK_PERIOD          500     // millisec

#define BATTERY_RATE_UPDATE       100     // check battery level
#define BATTERY_HISTO_BUFFER_SIZE 16      // for average value computing (warning consume RAM, for nano, memory is tiny)

  /*
   * R1, R2 resistor choices
   * 
   * For Arduino Nano
   * ----------------
   * 
   * Analog pins: VCC = 5v
   * 2S battery: VCC = 7.40v, R1 = 47000 Ohm, R2 = 95300 Ohm 1%
   * 3S battery: VCC = 11.1v, R1 = 47000 Ohm, R2 = 38300 Ohm 1%
   * 
   * For Teensy 3.2
   * --------------
   * Analog pins: VCC = 3.3v
   * 2S battery: VCC = 7.40v, R1 = 47000 Ohm, R2 = 37400 Ohm 1%
   * 3S battery, VCC = 11.1v, R1 = 27000 Ohm, R2 = 11000 Ohm 1%
   * 
   */
 
#define BATTERY_R1                27000  // Must be modity to you R1 resistor, ex: 27k Ohm
#define BATTERY_R2                11000  // Must be modity to you R2 resistor, ex: 11k Ohm
#define BATTERY_RAISE_ALARM_LEVEL 9.8    // Raise alarm when V batt is lower than 9.80 v


//
//
//  Put here after all definitions relative to your configuration
//
//

//#define TERRATOP
#define NEWRADIO
          
#define JETI_TU2_MODULE



#ifdef TERRATOP

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define MAX_DIG_INPUT_CHANNEL   3       // Mechanical switches

#define PPM_PIN                 10

#define SWITCH1_PIN             2
#define SWITCH2_PIN             3
#define SWITCH3_PIN             4

#define LED_PIN                 6

#else

#define MAX_ADC_INPUT_CHANNEL   4       // Gimballs or pot
#define MAX_DIG_INPUT_CHANNEL   2       // Mechanical switches

#define PPM_PIN                 10

#define SWITCH1_PIN             2
#define SWITCH2_PIN             3

#define LED_PIN                 13

#endif


//
//  HF Modules
//

#ifdef JETI_TU2_MODULE

#define MAX_PPM_OUTPUT_CHANNEL  6       // 17 channels max with Jeti TU2 module
#define PPM_SHAPE_SIGNAL        HIGH    // LOW is positive, HIGH is negative
#define PPM_INTER_CHANNEL_TIME  500     // microsec
#define PPM_INTER_FRAME_TIME    4500    // microsec

#endif

//////////////////////////////////////////
//    WARNING Never modify lower defines
//////////////////////////////////////////

#define MAX_INPUT_CHANNEL       MAX_ADC_INPUT_CHANNEL+MAX_DIG_INPUT_CHANNEL
#define STDOUT                  *SerialLink::currentStream_

#if defined(__MK20DX256__)
#define QUARKTX_TEENSY
#define VREF 3.3
#else
#define QUARKTX_NANO
#define VREF 5.0
#endif

#if defined(QT_CORE_LIB)
#define QUARKTX_TEST
#endif

#endif
