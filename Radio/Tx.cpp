/*
Tx.cpp - QuarkTx
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

#include <EEPROM.h>
#ifdef QUARKTX_TEENSY
#include "mk20dx128.h"
#endif
#include "Config.h"
#include "SerialLink.h"
#include "Tx.h"


Tx::Tx()
   :
     inFreq_(-1),
     outFreq_(-1),
     inCurFreq_(0),
     outCurFreq_(0),
     ledState_(HIGH),
     toggleTxMode_(tTransmit),
     toggleDisplayInputUpdate_(false),
     toggleDisplayOutputUpdate_(false),
     toggleCalibrateSensor_(false)
{
//   for(uint8_t i=0; i < MAX_MODEL; i++)
//      modelList_[i] = new Model;

   currentModel_ = &modelList_[0];

   for(uint8_t i=0; i < MAX_ADC_INPUT_CHANNEL; i++)
      sensor_[i] = new Stick;

   for(uint8_t i=MAX_ADC_INPUT_CHANNEL; i < MAX_ADC_INPUT_CHANNEL+MAX_DIG_INPUT_CHANNEL; i++)
      sensor_[i] = new Switch;

   onSoftwareReset("");
}

void Tx::setupInputDevice()
{
#ifndef QUARKTX_TEENSY

   // Uncomment below for speedup input scan frequence on Arduino Nano
   /*
   const unsigned char PS_16 = (1 << ADPS2);                                 // 1 MHz
   const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);                  // 500 KHz
   const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);                  // 250 KHz
   const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 125 KHz

   // set up the ADC
   ADCSRA &= ~PS_128;  // remove bits set by Arduino library
   ADCSRA |= PS_64;    // set our own prescaler to 64
  */
  
#endif

   sensor_[0]->setup(A0);
   sensor_[1]->setup(A1);
   sensor_[2]->setup(A2);
   sensor_[3]->setup(A3);
   sensor_[4]->setup(SWITCH1_PIN);
   sensor_[5]->setup(SWITCH2_PIN);
#ifdef TERRATOP
   sensor_[6]->setup(SWITCH3_PIN);
#endif

   battMeter_.setup(A7);
}

void Tx::setupOutputDevice()
{
   // PPM for RF module
   pinMode(PPM_PIN, OUTPUT);
   digitalWrite(PPM_PIN, PPM_SHAPE_SIGNAL);  //set the PPM signal pin to the default state
   
   // LED
   pinMode(LED_PIN, OUTPUT);
   digitalWrite(LED_PIN, HIGH);

   // Init irq variables
   irqStartPulse_ = true;
   irqCurrentChannelNumber_ = 0;

#ifdef QUARKTX_TEENSY

   SIM_SCGC6 |= SIM_SCGC6_PIT;  // Enable PIT clock
   PIT_MCR = 0x00;              // turn on PIT
   PIT_TCTRL1 = 0x00;           // stop Timer
   PIT_LDVAL1 = (F_BUS/1000000)*PPM_INTER_FRAME_TIME-1;
   PIT_TCTRL1 = 0x03;           // enable Timer interrupts + start
   PIT_TFLG1 = 1;
   NVIC_SET_PRIORITY(1, 128);
   NVIC_ENABLE_IRQ(IRQ_PIT_CH1);
   
#else

   cli();
   
   TCCR1A = 0;               // set entire TCCR1 register to 0
   TCCR1B = 0;

   OCR1A = PPM_INTER_FRAME_TIME*2;  // compare match register, initial value

   TCCR1B |= (1 << WGM12);   // turn on CTC mode
   TCCR1B |= (1 << CS11);    // prescaler to 8 -> 0.5 microseconds at 16mhz

   TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

   sei();
   
#endif
}

bool Tx::setup()
{
   mesure_.start();

   // For battery extended duration put all unused pin off
   pinMode(2, INPUT_PULLUP);
   pinMode(3, INPUT_PULLUP);
   pinMode(4, INPUT_PULLUP);
   pinMode(5, INPUT_PULLUP);
   pinMode(6, INPUT_PULLUP);
   pinMode(7, INPUT_PULLUP);
   pinMode(8, INPUT_PULLUP);
   pinMode(9, INPUT_PULLUP);
   pinMode(10, INPUT_PULLUP);
   pinMode(11, INPUT_PULLUP);
   pinMode(12, INPUT_PULLUP);

   // Analog pins
   pinMode(A0, INPUT);           // gimbal 1
   pinMode(A1, INPUT);           // gimbal 2
   pinMode(A2, INPUT);           // gimbal 3
   pinMode(A3, INPUT);           // gimbal 4
   pinMode(A4, INPUT_PULLUP);    // reseved for future use
   pinMode(A5, INPUT_PULLUP);    // reseved for future use
   pinMode(A6, INPUT_PULLUP);    // reseved for future use
   pinMode(A7, INPUT_PULLUP);    // Battery level

#ifdef QUARKTX_TEENSY
   pinMode(A8, INPUT_PULLUP);    // reseved for future use
   pinMode(A9, INPUT_PULLUP);    // reseved for future use
   pinMode(A10, INPUT_PULLUP);    // reseved for future use
   pinMode(A11, INPUT_PULLUP);    // reseved for future use
   pinMode(A12, INPUT_PULLUP);    // reseved for future use
   pinMode(A13, INPUT_PULLUP);    // reseved for future use
   pinMode(A14, INPUT_PULLUP);    // reseved for future use
#endif

  //analogReference(INTERNAL);
  analogReference(DEFAULT);

#ifdef QUARKTX_TEENSY
  analogReadRes(10);
  analogReadAveraging(4);
#endif

   // serial must always be first to initialize
   bool ret1 = serialLink_.setup(&command_);
   
   // Setup input sensors
   setupInputDevice();

   // Setup Timer for PPM signal generation
   setupOutputDevice();
   bool ret2 = command_.setup(this);
   
   rcl_.setup(sensor_, ppmOutputValue_, currentModel_);
   
   onLoadFromEEPROM();
   
   /*
  rcl_.setupRCL(0, "i0");
  rcl_.setupRCL(1, "i1");
  rcl_.setupRCL(2, "i2");
  rcl_.setupRCL(3, "i3");
  rcl_.setupRCL(4, "i4[0;512]+i5[512;0]");
#ifdef TERRATOP
  rcl_.setupRCL(5, "(i2>512)?i0:0");
#endif
*/
   mesure_.stop();
   STDOUT << F("Tx\t\tOK\n") << mesure_.getAverage() << F(" µs") << endl;

#ifdef QUARKTX_TEENSY
   STDOUT << F("Teensy 3.2") << endl; // Teensy signnature
#else
   STDOUT << F("Nano") << endl; // Nano signnature
#endif

   return ret1 | ret2;
}

void Tx::onIsrTimerChange()
{
   /*
   * With new 2.4 GHz HF modules, we dont care anymore about 20 ms constraint
   * (as HF modules can now sent more than 8 channels) but just care about 2 times:
   * The minimum time between 2 channels pulses (PPM_INTER_CHANNEL_TIME)
   * The minimum time between 2 PPM frames (PPM_INTER_FRAME_TIME)
   */
   
#ifdef QUARKTX_TEENSY

   PIT_TFLG1 = 1;
   
   if(toggleTxMode_ != tTransmit)
      return;

   if(irqStartPulse_)
   {
      // Falling edge of a channel pulse (in negative shape)
      digitalWrite(PPM_PIN, PPM_SHAPE_SIGNAL);
      
      PIT_LDVAL1 = (F_BUS/1000000)*ppmOutputValue_[irqCurrentChannelNumber_]-1;
      irqCurrentChannelNumber_++;
      irqStartPulse_ = false;
   }
   else
   {
      // Raising edge of a channel pulse (in negative shape)
      digitalWrite(PPM_PIN, !PPM_SHAPE_SIGNAL);

      // Calculate when the next pulse will start
      if(irqCurrentChannelNumber_ >= MAX_PPM_OUTPUT_CHANNEL)
      {
         irqCurrentChannelNumber_ = 0;
         PIT_LDVAL1 = (F_BUS/1000000)*PPM_INTER_FRAME_TIME-1;
      }
      else
         PIT_LDVAL1 = (F_BUS/1000000)*PPM_INTER_CHANNEL_TIME-1;

      irqStartPulse_ = true;
   }

#else
   /*
   * We successfully test with a Jeti TU2 RF module up to 17 channels
   */

   TCNT1 = 0;
   
   if(toggleTxMode_ != tTransmit)
      return;
      
   if(irqStartPulse_)
   {
      // Falling edge of a channel pulse (in negative shape)
      digitalWrite(PPM_PIN, !PPM_SHAPE_SIGNAL);

      // All time must be x2, as prescale is set to 0.5 microseconds at 16mhz
      OCR1A = ppmOutputValue_[irqCurrentChannelNumber_]*2;
      irqCurrentChannelNumber_++;
      irqStartPulse_ = false;
   }
   else
   {
      // Raising edge of a channel pulse (in negative shape)
      digitalWrite(PPM_PIN, PPM_SHAPE_SIGNAL);

      // Calculate when the next pulse will start
      // and put this time in OCR1A (x2 as prescaler is set to 0.5 microsec)
      if(irqCurrentChannelNumber_ >= MAX_PPM_OUTPUT_CHANNEL)
      {
         irqCurrentChannelNumber_ = 0;
         OCR1A = PPM_INTER_FRAME_TIME*2;
      }
      else
         OCR1A = PPM_INTER_CHANNEL_TIME*2;

      irqStartPulse_ = true;
   }
#endif
}

void Tx::idle()
{
   if(toggleTxMode_ == tDebug)
      mesure_.start();

   rcl_.idle();
   ledBlinkIdle();
   serialLink_.idle();

   if(toggleDisplayInputUpdate_)
      displayInputUpdate();
   if(toggleDisplayOutputUpdate_)
      displayOutputUpdate();
   if(toggleCalibrateSensor_)
      calibrateSensor();
   if(battMeter_.checkLevelTooLow())
      onRaiseBatteryAlarm();

   if(toggleTxMode_ == tDebug)
   {
      mesure_.stop();
      mesure_.displayStat(1000);
   }
   
   // 1200 1218 1380
}

void Tx::ledBlinkIdle()
{
   if(toggleTxMode_ == tTransmit)
   {
      unsigned long cur = millis();

      if(cur - ledPrevMS_ >= LED_BLINK_PERIOD)
      {
         ledPrevMS_ = cur;
         ledState_ = (ledState_ == LOW)?HIGH:LOW;

         digitalWrite(LED_PIN, ledState_);
      }
   }
   else
      digitalWrite(LED_PIN, HIGH);
}

void Tx::onRaiseBatteryAlarm()
{
  //STDOUT << "Low battery" << endl;
}

void Tx::displayInputUpdate()
{
   if(inFreq_ > 0)
   {
      if(inCurFreq_++ < inFreq_)
         return;
      inCurFreq_ = 0;
   }
   
   STDOUT << F("<\t");

   for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      STDOUT << sensor_[idx]->getValue() << F("\t");

   STDOUT << battMeter_.getAverageValueInVolt() << F("\t") << endl;
}

void Tx::onToggleDisplayInputUpdate(int freq)
{
   inCurFreq_ = 0;
   inFreq_ = freq;
   toggleDisplayInputUpdate_ = !toggleDisplayInputUpdate_;
}

void Tx::displayOutputUpdate()
{
   if(outFreq_ > 0)
   {
      if(outCurFreq_++ < outFreq_)
         return;
      outCurFreq_ = 0;
   }
   
   STDOUT << F(">\t");

   for(uint8_t idx = 0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      STDOUT << ppmOutputValue_[idx] << "\t";

   STDOUT << endl;
}

void Tx::onToggleDisplayOutputUpdate(int freq)
{
   outFreq_ = freq;
   toggleDisplayOutputUpdate_ = !toggleDisplayOutputUpdate_;
}

void Tx::onToggleTxMode()
{
   if(toggleTxMode_ == tTransmit)
   {
      toggleTxMode_ = tDebug;
      STDOUT << F("Mode 'debug'") << endl;
   }
   else
   {
      toggleTxMode_ = tTransmit;
      STDOUT << F("Mode 'transmit'") << endl;
   }

   mesure_.reset();
}

void Tx::onChangeCurrentModel(int idx)
{
   if(idx < MAX_MODEL)
   {
      STDOUT << F("Current model ") << idx << endl;
      currentModel_ = &modelList_[idx];
      rcl_.changeModel(currentModel_);
   }
   else
      STDOUT << F("e-bp ") << idx << F(" ") << MAX_MODEL-1 << endl;  // Bad parameter
}

void Tx::dumpEEPROM()
{
   for(int idx=0, i=0; idx < EEPROM.length(); idx++,i++)
   {
      if(i == 0)
      {
         if(idx < 0x10)
            STDOUT << F("00");
         else if (idx < 0x100)
            STDOUT << F("0");

         STDOUT << _HEX(idx) << "\t";
      }

      uint8_t v = EEPROM.read(idx);
      if(v <= 0xF)
         STDOUT << F("0");
      STDOUT << _HEX(v) << " ";

      if(i == 15)
      {
         i = -1;
         STDOUT << endl;
      }
   }
   STDOUT << endl;
}

void Tx::dumpModel()
{
   for(uint8_t idx=0; idx < MAX_MODEL; idx++)
   {
      char c = (currentModel_ == &modelList_[idx])?'*':' ';
      STDOUT << F("Model ") << idx << " " << c << endl;

      modelList_[idx].dump();
   }
}

void Tx::dumpSensor()
{
   STDOUT << F("Sensors (") << MAX_INPUT_CHANNEL << F(")\n# Pin   Trim    Min     Max") << endl;

   // Sensors
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
   {
      STDOUT << idx << " ";
      sensor_[idx]->dump();
      STDOUT << endl;
   }

   // Battery meter
   STDOUT << F("b ");
   battMeter_.dump();
   
   STDOUT << endl << endl;
}

void Tx::dumpRCL(const char* param)
{
   if(param[0] == 0)
   {
      for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      {
         STDOUT << "# " << idx << endl;
         rcl_.dump(idx);
         STDOUT << endl;
      }
   }
   else
   {
      rcl_.dump(atoi(param+2));
      STDOUT << endl;
   }
}

void Tx::onDump(const char* param)
{
   switch(param[0])
   {
      case 'e': dumpEEPROM();break;
      case 'l': dumpRCL(param+1);break;
      case 'm': dumpModel();break;
      case 's': dumpSensor();break;
      default:
         dumpSensor();
         dumpModel();
         dumpRCL("");
         dumpEEPROM();
   }
}

void Tx::onToggleCalibrateSensor()
{
   toggleCalibrateSensor_ = !toggleCalibrateSensor_;
}

void Tx::calibrateSensor()
{  
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
   {
      sensor_[idx]->calibrate();

      STDOUT << "{" << sensor_[idx]->getMinCalibration() << "\t" << sensor_[idx]->getMaxCalibration() << "}\t";
   }

   STDOUT << endl;
}

bool Tx::onLoadFromEEPROM()
{
   uint16_t addr = 0L;
   uint8_t i;

   // get current model used index
   EEPROM.get(addr, i);
   if(i >= MAX_MODEL)    // EEPROM is corrupted
   {
      i = 0;
      STDOUT << F("e-edic") << endl;    //  EEPROM data is corrupted
      return false;
   }
   currentModel_ = &modelList_[i];
   addr += sizeof(uint8_t);

   // get Model data
   for(uint8_t idx=0; idx < MAX_MODEL; idx++)
      modelList_[idx].loadFromEEPROM(addr);

   // get Sensors data
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->loadFromEEPROM(addr);

   rcl_.loadFromEEPROM(addr);

   return true;
}

void Tx::onSaveToEEPROM()
{
   uint16_t addr = 0L;
   uint8_t i = getModelIndex(currentModel_);

   // save current model index
   EEPROM.put(addr, i);
   addr += sizeof(uint8_t);

   // save Model data
   for(uint8_t idx=0; idx < MAX_MODEL; idx++)
      modelList_[idx].saveToEEPROM(addr);

   // save Sensor data
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->saveToEEPROM(addr);

   rcl_.saveToEEPROM(addr);
}

void Tx::resetModel()
{
   for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
      modelList_[idx].reset();

   currentModel_ = &modelList_[0];
}

void Tx::resetSensor()
{
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->reset();
      
   battMeter_.reset();
}

void Tx::resetRCL()
{
   // Clean all
   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
      rcl_.clearRCL(idx);

   // Set to default
   char buf[3];
   buf[0] = 'i';
   buf[2] = 0;

   uint8_t i=0;
   for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      if(i >= MAX_INPUT_CHANNEL)
         i = 0;

      buf[1] = '0'+i;
      rcl_.setupRCL(idx, buf);
      i++;
   }
}

void Tx::onSoftwareReset(const char* param)
{
   switch(param[0])
   {
      case 'm': resetModel();break;
      case 's': resetSensor();break;
      case 'l': resetRCL();break;
      default:
         resetModel();
         resetSensor();
         resetRCL();
   }

   toggleTxMode_ = tTransmit;

   setupInputDevice();
}

uint8_t Tx::getSensorIndex(uint8_t pinPort)
{
   for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
   {
      if(sensor_[idx]->getPin() == pinPort)
         return idx;
   }

   return -1;
}

uint8_t Tx::getModelIndex(Model *model)
{
   for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
   {
      if(model == &modelList_[idx])
         return idx;
   }
   return -1;
}

void Tx::onSetTrimSensorValue(uint8_t sensorID, int value)
{
   if(sensorID < 0) return;
   sensor_[sensorID]->setTrim(value);
}

void Tx::onSetMinSensorValue(uint8_t sensorID, uint16_t value)
{
   if(sensorID < 0) return;
   sensor_[sensorID]->setMin(value);
}

void Tx::onSetMaxSensorValue(uint8_t sensorID, uint16_t value)
{
   if(sensorID < 0) return;
   sensor_[sensorID]->setMax(value);
}

void Tx::onSetRCL(uint8_t chan, const char* rclCode)
{
   if(rclCode[0] == 0)
   {
      STDOUT << chan << F(" cleaned") << endl;
      rcl_.clearRCL(chan);
   }
   else
   {
      STDOUT << chan << " " << rclCode << endl;
      if(!rcl_.setupRCL(chan, rclCode))
         STDOUT << F("e-pf") << endl;       // RCL parsing failed
   }
}

#ifdef QUARKTX_TEST

void Tx::onNewCommand(const char* cmdStr)
{
   command_.onNewCommand(cmdStr);
}

void Tx::onEvaluateExpression()
{
   rcl_.idle();
}

#endif

