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


Tx::Tx() :
     isBootSeqAlreadyDisplayed_(false),
     sdLoad_(false),
     ledState_(HIGH),
     ledBlinkPeriod_(QUARKTX_LED_BLINK_PERIOD),
     toggleTxMode_(tTransmit),
     toggleDisplayInputUpdate_(false),
     toggleDisplayOutputUpdate_(false),
     toggleCalibrateSensor_(false)
{
//   for(uint8_t i=0; i < MAX_MODEL; i++)
//      modelList_[i] = new Model;

   currentModel_ = &modelList_[0];

   for(uint8_t i=0; i < QUARKTX_MAX_ADC_INPUT_CHANNEL; i++)
      sensor_[i] = new Stick;

   for(uint8_t i=QUARKTX_MAX_ADC_INPUT_CHANNEL; i < (QUARKTX_MAX_ADC_INPUT_CHANNEL + QUARKTX_MAX_DIG_INPUT_CHANNEL); i++)
      sensor_[i] = new Switch;

   onSoftwareReset("");
}

void Tx::setupInputDevice()
{
#ifdef QUARKTX_NANO

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

#if QUARKTX_TX_MODE == 1
   sensor_[0]->setup(A0);   // elevators
   sensor_[1]->setup(A1);   // rudder
   sensor_[2]->setup(A2);   // throttle
   sensor_[3]->setup(A3);   // ailerons
#elif QUARKTX_TX_MODE == 2
   sensor_[0]->setup(A2);   // throttle
   sensor_[1]->setup(A1);   // rudder
   sensor_[2]->setup(A0);   // elevators
   sensor_[3]->setup(A3);   // ailerons
#elif QUARKTX_TX_MODE == 3
   sensor_[0]->setup(A0);   // elevators
   sensor_[1]->setup(A3);   // ailerons
   sensor_[2]->setup(A2);   // throttle
   sensor_[3]->setup(A1);   // rudder
#else
   sensor_[0]->setup(A2);   // throttle
   sensor_[1]->setup(A3);   // ailerons
   sensor_[2]->setup(A0);   // elevators
   sensor_[3]->setup(A1);   // rudder
#endif
   
#ifdef SWITCH1_PIN
   sensor_[4]->setup(SWITCH1_PIN);
#endif   
#ifdef SWITCH2_PIN
   sensor_[5]->setup(SWITCH2_PIN);
#endif
#ifdef SWITCH3_PIN
   sensor_[6]->setup(SWITCH3_PIN);
#endif

   battMeter_.setup(A7);
}

void Tx::setupOutputDevice()
{
   // PPM for RF module
   pinMode(QUARKTX_PPM_PIN, OUTPUT);
   digitalWrite(QUARKTX_PPM_PIN, QUARKTX_PPM_SHAPE_SIGNAL);  //set the PPM signal pin to the default state
   
   // LED
   pinMode(QUARKTX_LED_PIN, OUTPUT);
   digitalWrite(QUARKTX_LED_PIN, HIGH);

   // Init irq variables
   isrStartPulse_ = true;
   isrCurrentChannelNumber_ = 0;

#ifdef QUARKTX_TEENSY

   SIM_SCGC6 |= SIM_SCGC6_PIT;  // Enable PIT clock
   PIT_MCR = 0x00;              // turn on PIT
   PIT_TCTRL1 = 0x00;           // stop Timer
   PIT_LDVAL1 = (F_BUS/1000000)*QUARKTX_PPM_INTER_FRAME_TIME-1;
   PIT_TCTRL1 = 0x03;           // enable Timer interrupts + start
   PIT_TFLG1 = 1;
   NVIC_SET_PRIORITY(1, 128);
   NVIC_ENABLE_IRQ(IRQ_PIT_CH1);
   
#else

   cli();
   
   TCCR1A = 0;               // set entire TCCR1 register to 0
   TCCR1B = 0;

   OCR1A = QUARKTX_PPM_INTER_FRAME_TIME*2;  // compare match register, initial value

   TCCR1B |= (1 << WGM12);   // turn on CTC mode
   TCCR1B |= (1 << CS11);    // prescaler to 8 -> 0.5 microseconds at 16mhz

   TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

   sei();
   
#endif
}

void Tx::setup()
{
   // For battery duration put all unused pin in pullup mode

   // Digital pins
   for(uint8_t i = 0; i < 13; i++)
      pinMode(i, INPUT_PULLUP);

   // Analog pins
   for(uint8_t i = A0; i != A7; i++)
      pinMode(i, INPUT_PULLUP);

#ifdef QUARKTX_TEENSY
   pinMode(A8,  INPUT_PULLUP);   // reseved for future use A10
   pinMode(A9,  INPUT_PULLUP);   // reseved for future use AREF
   pinMode(A10, INPUT_PULLUP);   // reseved for future use A8
   pinMode(A11, INPUT);          // reseved for future use A7
   pinMode(A12, INPUT_PULLUP);   // reseved for future use
   pinMode(A13, INPUT_PULLUP);   // reseved for future use
   pinMode(A14, INPUT_PULLUP);   // reseved for future use
#endif

   analogReference(DEFAULT);      // Arduino nano Vref = 5v, Teensy 3.2 Vref = 3.3

#ifdef QUARKTX_TEENSY
   analogReadRes(10);
  //analogReadAveraging(16);
#endif

   // serial must always be first to initialize
   serialOk_ = serialLink_.setup(&command_);
   if(!serialLink_.isPrimaryActive())
      ledBlinkPeriod_ = QUARKTX_LED_BLINK_PERIOD/2;
   
   // Setup input sensors
   setupInputDevice();

   // Setup Timer for PPM signal generate
   setupOutputDevice();
   command_.setup(this);
   
   rcl_.setup(sensor_, ppmOutputValue_, currentModel_, this);

#ifdef QUARKTX_TEENSY
   sdOk_ = fs_.setup();
   sdLoad_ = onLoadFromFile();
#endif

   if(!sdLoad_)
      onLoadFromEEPROM();
   
/*
  rcl_.setupRCL(0, "i0");
  rcl_.setupRCL(1, "i1");
  rcl_.setupRCL(2, "i2");
  rcl_.setupRCL(3, "i3");
  rcl_.setupRCL(4, "i4[0;512]+i5[512;0]");
*/
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

   if(isrStartPulse_)
   {
      // Falling edge of a channel pulse (in negative shape)
      digitalWrite(QUARKTX_PPM_PIN, QUARKTX_PPM_SHAPE_SIGNAL);
      
      PIT_LDVAL1 = (F_BUS/1000000)*ppmOutputValue_[isrCurrentChannelNumber_]-1;
      isrCurrentChannelNumber_++;
      isrStartPulse_ = false;
   }
   else
   {
      // Raising edge of a channel pulse (in negative shape)
      digitalWrite(QUARKTX_PPM_PIN, !QUARKTX_PPM_SHAPE_SIGNAL);

      // Calculate when the next pulse will start
      if(isrCurrentChannelNumber_ >= QUARKTX_MAX_PPM_OUTPUT_CHANNEL)
      {
         isrCurrentChannelNumber_ = 0;
         PIT_LDVAL1 = (F_BUS/1000000)*QUARKTX_PPM_INTER_FRAME_TIME-1;
      }
      else
         PIT_LDVAL1 = (F_BUS/1000000)*QUARKTX_PPM_INTER_CHANNEL_TIME-1;

      isrStartPulse_ = true;
   }

#else
  /*
   * We successfully test with a Jeti TU2 RF module up to 17 channels
   */

   TCNT1 = 0;
   
   if(toggleTxMode_ != tTransmit)
      return;
      
   if(isrStartPulse_)
   {
      // Falling edge of a channel pulse (in negative shape)
      digitalWrite(QUARKTX_PPM_PIN, !QUARKTX_PPM_SHAPE_SIGNAL);

      // All time must be x2, as prescale is set to 0.5 microseconds at 16mhz
      OCR1A = ppmOutputValue_[isrCurrentChannelNumber_]*2;
      isrCurrentChannelNumber_++;
      isrStartPulse_ = false;
   }
   else
   {
      // Raising edge of a channel pulse (in negative shape)
      digitalWrite(QUARKTX_PPM_PIN, QUARKTX_PPM_SHAPE_SIGNAL);

      // Calculate when the next pulse will start
      // and put this time in OCR1A (x2 as prescaler is set to 0.5 microsec)
      if(isrCurrentChannelNumber_ >= QUARKTX_MAX_PPM_OUTPUT_CHANNEL)
      {
         isrCurrentChannelNumber_ = 0;
         OCR1A = QUARKTX_PPM_INTER_FRAME_TIME*2;
      }
      else
         OCR1A = QUARKTX_PPM_INTER_CHANNEL_TIME*2;

      isrStartPulse_ = true;
   }
#endif
}

void Tx::loop()
{
  /*
   * This is the main loop, only non blocking code must be put here.
   */
   currentMs_ = millis();
   
   if(toggleTxMode_ == tDebug)
      mesure_.start();

   rcl_.loop();
   ledBlinkUpdate();
   serialLink_.loop();
   battLevelCheck();
    
   if(isBootSeqAlreadyDisplayed_ == false)
      displayBootingSequence();
   if(toggleDisplayInputUpdate_)
      displayInputUpdate();
   if(toggleDisplayOutputUpdate_)
      displayOutputUpdate();
   if(toggleCalibrateSensor_)
      calibrateSensor();

   if(toggleTxMode_ == tDebug)
   {
      mesure_.stop();
      mesure_.displayStat(1000);
   }
   
   // 1200 1218 1380
}

void Tx::displayBootingSequence()
{
   if(isBootSeqAlreadyDisplayed_ == true)
      return;

   isBootSeqAlreadyDisplayed_ = true;

   STDOUT << F("Quark Tx v") << F(QUARKTX_VERSION) << endl;
#ifdef QUARKTX_TEENSY
   STDOUT << F("Teensy 3.2 platform\nLua inside") << endl;
#else
   STDOUT << F("Nano platform") << endl;
#endif
   STDOUT << F("Booting...") << endl;
   STDOUT << F("Serial\t\t");
   if(serialOk_) STDOUT << F("OK") << endl; else STDOUT << F("KO") << endl;
   STDOUT << F("Command\t\tOK") << endl;
#ifdef QUARKTX_TEENSY
   STDOUT << F("SD reader\t");
   if(sdOk_) STDOUT << F("OK") << endl; else STDOUT << F("KO") << endl;
#endif
   STDOUT << F("Tx\t\tOK\nReady") << endl;

   serialLink_.displayPrompt();
}

void Tx::ledBlinkUpdate()
{
  /*
   * If the Led is blicking every LED_BLINK_PERIOD
   * Visual check that main loop is not freezed
   */
   
   if(toggleTxMode_ == tTransmit)
   {
      if(currentMs_ - ledPrevMs_ >= ledBlinkPeriod_)
      {
         ledPrevMs_ = currentMs_;
         ledState_ = (ledState_ == LOW)?HIGH:LOW;

         digitalWrite(QUARKTX_LED_PIN, ledState_);
      }
   }
   else
      digitalWrite(QUARKTX_LED_PIN, HIGH);
}

void Tx::battLevelCheck()
{
  if(currentMs_ - battPrevMs_ >= BATTERY_RATE_PERIOD)
  {
    battPrevMs_ = currentMs_;
    battMeter_.isLevelTooLow();  // todo generate sound
  }
}

void Tx::displayInputUpdate()
{
   if(currentMs_ - displayInputPrevMs_ < displayInputPeriod_)
      return;
   
   displayInputPrevMs_ = currentMs_;
   
   STDOUT << F("<\t");

   for(uint8_t idx = 0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
      STDOUT << sensor_[idx]->getValue() << F("\t");

   STDOUT << battMeter_.getLastAverageValueInVolt() << F("\t") << endl;
}

void Tx::onToggleDisplayInputUpdate(int period)
{
   displayInputPeriod_ = period;
   toggleDisplayInputUpdate_ = !toggleDisplayInputUpdate_;
}

void Tx::displayOutputUpdate()
{
   if(currentMs_ - displayOutputPrevMs_ < displayOutputPeriod_)
      return;
      
    displayOutputPrevMs_ = currentMs_;
    
    STDOUT << F(">\t");
    
    for(uint8_t idx = 0; idx < QUARKTX_MAX_PPM_OUTPUT_CHANNEL; idx++)
      STDOUT << ppmOutputValue_[idx] << "\t";
    
    STDOUT << endl;
}

void Tx::onToggleDisplayOutputUpdate(int period)
{
   displayOutputPeriod_ = period;
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
   if(idx < QUARKTX_MAX_MODEL)
   {
      STDOUT << F("Current model ") << idx << endl;
      currentModel_ = &modelList_[idx];
      rcl_.changeModel(currentModel_);
   }
   else
      STDOUT << F("e-bp ") << idx << F(" ") << QUARKTX_MAX_MODEL-1 << endl;  // Bad parameter
}

void Tx::dumpEEPROM()
{
  /*
   * Warning: on Teensy 3.2 EEPROM.length() is 0x7F0 long.
   * for() loop could interrupt main refresh update if Serial
   * is linked to Bluetooth 9600 bauds low baud rate
   */
   
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
   for(uint8_t idx=0; idx < QUARKTX_MAX_MODEL; idx++)
   {
      char c = (currentModel_ == &modelList_[idx])?'*':' ';
      STDOUT << F("Model ") << idx << " " << c << endl;

      modelList_[idx].dump();
   }
}

void Tx::dumpSensor()
{
   STDOUT << F("Sensors (") << QUARKTX_MAX_INPUT_CHANNEL << F(")\n# Pin   Trim    Min     Max") << endl;

   // Sensors
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
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
      for(uint8_t idx=0; idx < QUARKTX_MAX_PPM_OUTPUT_CHANNEL; idx++)
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

#ifdef QUARKTX_TEENSY
void Tx::dumpDirectory(const char* param)
{
  fs_.dumpDirectory(param);
}
#endif

void Tx::onDump(const char* param)
{
   switch(param[0])
   {
      case 'e': dumpEEPROM();break;
      case 'l': dumpRCL(param+1);break;
      case 'm': dumpModel();break;
      case 's': dumpSensor();break;
#ifdef QUARKTX_TEENSY
      case 'd': dumpDirectory(param+2);break;
#endif
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
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
   {
      sensor_[idx]->calibrate();

      STDOUT << "{" << sensor_[idx]->getMinCalibration() << "\t" << sensor_[idx]->getMaxCalibration() << "}\t";
   }

   STDOUT << endl;
}

#ifdef QUARKTX_TEENSY
 bool Tx::onLoadFromFile()
 {
    DynamicJsonBuffer buf;

    int file = fs_.open("/MODEL/0001ORCA.json");
    const char *str = fs_.read(file);

    STDOUT << str;
    
    JsonObject& root = buf.parseObject(str);

    root.printTo(Serial);
    
    return false;
 }
 
 bool Tx::onSaveToFile()
 {
    DynamicJsonBuffer buf;

    return false;
 }
#endif

bool Tx::onLoadFromEEPROM()
{
   uint16_t addr = 0x0000;
   uint16_t eepromFormatVersion;
   uint8_t i;

   EEPROM.get(addr, eepromFormatVersion);
   if(eepromFormatVersion != QUARKTX_EEPROM_FORMAT)
   {
      STDOUT << F("e-bef ") << eepromFormatVersion << " " << QUARKTX_EEPROM_FORMAT << endl;  // Bad EEPROM format version
      return false;
   }
   addr+= sizeof(uint16_t);

   // get current model used index
   EEPROM.get(addr, i);
   if(i >= QUARKTX_MAX_MODEL)    // EEPROM is corrupted
   {
      i = 0;
      STDOUT << F("e-edic") << endl;    //  EEPROM data is corrupted
      return false;
   }
   currentModel_ = &modelList_[i];
   addr += sizeof(uint8_t);

   // get Model data
   for(uint8_t idx=0; idx < QUARKTX_MAX_MODEL; idx++)
      modelList_[idx].loadFromEEPROM(addr);

   // get Sensors data
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->loadFromEEPROM(addr);

   battMeter_.loadFromEEPROM(addr);

   rcl_.loadFromEEPROM(addr);

   return true;
}

void Tx::onSaveToEEPROM()
{
   uint16_t addr = 0x0000;
   uint16_t eepromFormatVersion = QUARKTX_EEPROM_FORMAT;
   uint8_t i = getModelIndex(currentModel_);

   EEPROM.put(addr, eepromFormatVersion);
   addr += sizeof(uint16_t);

   // save current model index
   EEPROM.put(addr, i);
   addr += sizeof(uint8_t);

   // save Model data
   for(uint8_t idx=0; idx < QUARKTX_MAX_MODEL; idx++)
      modelList_[idx].saveToEEPROM(addr);

   // save Sensor data
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->saveToEEPROM(addr);

   battMeter_.saveToEEPROM(addr);

   rcl_.saveToEEPROM(addr);
}

void Tx::resetModel()
{
   for(uint8_t idx = 0; idx < QUARKTX_MAX_MODEL; idx++)
      modelList_[idx].reset();

   currentModel_ = &modelList_[0];
}

void Tx::resetSensor()
{
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->reset();
      
   battMeter_.reset();
}

void Tx::resetRCL()
{
   // Clean all
   for(uint8_t idx=0; idx < QUARKTX_MAX_PPM_OUTPUT_CHANNEL; idx++)
      rcl_.clearRCL(idx);

   // Set to default
   char buf[3];
   buf[0] = 'i';
   buf[2] = 0;

   uint8_t i=0;
   for(uint8_t idx=0; idx < QUARKTX_MAX_PPM_OUTPUT_CHANNEL; idx++)
   {
      if(i >= QUARKTX_MAX_INPUT_CHANNEL)
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

uint8_t Tx::getSensorIndex(uint8_t pinPort) const
{
   for(uint8_t idx=0; idx < QUARKTX_MAX_INPUT_CHANNEL; idx++)
   {
      if(sensor_[idx]->getPin() == pinPort)
         return idx;
   }

   return -1;
}

uint8_t Tx::getModelIndex(Model *model)
{
   for(uint8_t idx = 0; idx < QUARKTX_MAX_MODEL; idx++)
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

void Tx::onSetTrimBattery(int value)
{
  battMeter_.setTrim(value);
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

