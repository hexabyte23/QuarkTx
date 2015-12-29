/*
Tx.cpp - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

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
#include "Config.h"
#include "SerialLink.h"
#include "Tx.h"
#include "FlashMem.h"


Tx::Tx()
:currentModel_(&modelList_[0]),
toggleMode_(tTransmit),
ledState_(LOW),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false),
toggleCalibrateSensor_(false),
toggleSimulation_(false)
{
  onReset();
}

void Tx::setupInputDevice()
{
  const unsigned char PS_16 = (1 << ADPS2);                                 // 1 MHz
  const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);                  // 500 KHz
  const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);                  // 250 KHz
  const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 125 KHz

  // set up the ADC
  //ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  //ADCSRA |= PS_64;    // set our own prescaler to 64 

  elevator_.setup(A0);
  aileron_.setup(A1);
  rudder_.setup(A2);
  throttle_.setup(A3);
  s1_.setup(SWITCH1_PIN);
  s2_.setup(SWITCH2_PIN);
#ifdef TERRATOP
  s3_.setup(SWITCH3_PIN);
#endif
  battery_.setup(A7);
}

void Tx::setupOutputDevice()
{
  // PPM for RF module
  pinMode(PPM_PIN, OUTPUT);
  digitalWrite(PPM_PIN, PPM_SHAPE_SIGNAL);  //set the PPM signal pin to the default state
  
  cli();
  
  TCCR1A = 0;               // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = PPM_INTER_FRAME_TIME;  // compare match register, initial value

  TCCR1B |= (1 << WGM12);   // turn on CTC mode
  TCCR1B |= (1 << CS11);    // prescaler to 8 -> 0,5 microseconds at 16mhz
 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  // init irq variables
  irqStartPulse_ = true;
  irqCurrentChannelNumber_ = 0;
  
  sei();

  // LED
  pinMode(LED_PIN, OUTPUT);
}

bool Tx::setup()
{
  mesure_.start();
  
  // for battery extended duration put all unused pin off
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
  pinMode(A7, INPUT_PULLUP);    // Battery level

  // Setup input sensors
  setupInputDevice();
  
  // Setup Timer for PPM signal generation
  setupOutputDevice();

  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

  onLoadFromEEPROM();

  evaluator_.setup(sensor_, ppmOutputValue_, currentModel_);
  evaluator_.setupOutputChannel(0, "i0");
  evaluator_.setupOutputChannel(1, "i1");
  evaluator_.setupOutputChannel(2, "i2");
  evaluator_.setupOutputChannel(3, "i3");
  evaluator_.setupOutputChannel(4, "i4[0;512]+i5[512;0]");
#ifdef TERRATOP
  evaluator_.setupOutputChannel(5, "i6[1023;0]");
#endif
  
  mesure_.stop();
  info(INFO_TX_READY,mesure_.getAverage());
  
  return ret1 | ret2;
}

void Tx::onIrqTimerChange()
{
  /*
   * With new 2.4 GHz HF modules, we dont care anymore about 20 ms constraint 
   * (as HF modules can now sent more than 8 channels) but just care about 2 times:
   * The minimum time between 2 channels pulses (PPM_INTER_CHANNEL_TIME) 
   * The minimum time between 2 PPM frames (PPM_INTER_FRAME_TIME)
   */
 
  TCNT1 = 0;

  if(irqStartPulse_) 
  {  
    // Falling edge of a channel pulse
    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, !PPM_SHAPE_SIGNAL);

    OCR1A = ppmOutputValue_[irqCurrentChannelNumber_]*2;
    irqCurrentChannelNumber_++;
    irqStartPulse_ = false;
  }
  else
  {
    // Raising edge of a channel pulse
    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, PPM_SHAPE_SIGNAL);
    irqStartPulse_ = true;

    // Calculate when the next pulse will start 
    // and put this time in OCR1A (x2 as prescaler is set to 0.5 microsec)
    if(irqCurrentChannelNumber_ >= MAX_PPM_OUTPUT_CHANNEL)
    {
      irqCurrentChannelNumber_ = 0;
      OCR1A = PPM_INTER_FRAME_TIME*2;
    }
    else
      OCR1A = PPM_INTER_CHANNEL_TIME*2;
  }
}

void Tx::idle()
{
  if(toggleMode_ == tDebug)
    mesure_.start();

  evaluator_.idle();
  ledBlinkIdle();
  serialLink_.idle();

  if(toggleDisplayInputUpdate_)
    displayInputUpdate();
  if(toggleDisplayOutputUpdate_)
    displayOutputUpdate();
  if(toggleCalibrateSensor_)
    calibrateSensor();

  if(toggleMode_ == tDebug)
  {
    mesure_.stop();
    mesure_.displayStat(1000);
  }
// 1200 1218 1380
}

void Tx::ledBlinkIdle()
{
  if(toggleMode_ == tTransmit)
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

void Tx::displayInputUpdate()
{
  STDOUT << "<\t";
  
  for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
    STDOUT << sensor_[idx]->getValue() << "\t";

  STDOUT << endl;
}

void Tx::onToggleDisplayInputUpdate()
{
  toggleDisplayInputUpdate_ = !toggleDisplayInputUpdate_;
}

void Tx::displayOutputUpdate()
{
  STDOUT << ">\t";

  for(uint8_t idx = 0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    STDOUT << ((toggleMode_ == tTransmit)?ppmOutputValue_[idx]:0) << "\t";

  STDOUT << endl;
}

void Tx::onToggleDisplayOutputUpdate()
{
  toggleDisplayOutputUpdate_ = !toggleDisplayOutputUpdate_;
}

void Tx::onToggleMode()
{
  if(toggleMode_ == tTransmit)
  {
    toggleMode_ = tDebug;
    info(INFO_SWITCH_MODE_SETTINGS);
    
    mesure_.reset();
  }
  else
  {
    toggleMode_ = tTransmit;
    info(INFO_SWITCH_MODE_TRANSMIT);
    
    mesure_.reset();
  }
}

void Tx::onChangeCurrentModel(int idx)
{
  if(idx < MAX_MODEL)
  {
    info(INFO_LOAD_MODEL,idx);
    currentModel_ = &modelList_[idx];
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onDump(const char* param)
{
  if(param[0] == 'e')
  {
    for(int idx=0, i=0; idx < EEPROM.length(); idx++,i++)
    {
      if(i == 0)
        STDOUT << _HEX(idx) << "\t";
      
      STDOUT << _HEX(EEPROM.read(idx)) << " ";
    
      if(i == 15)
      {
        i = -1;
        STDOUT << endl;
      }
    }
    STDOUT << endl;
  }
  else
  {
    // dump sensors
    info(INFO_SENSOR, MAX_INPUT_CHANNEL);

    for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    {
      STDOUT << idx << " ";
      sensor_[idx]->dump();
      STDOUT << endl;
    }
    STDOUT << endl;
    
    // dump models
    for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    {
      char c = (currentModel_ == &modelList_[idx])?'*':' ';
      STDOUT << "Model " << idx << " " << c << endl;
    
      modelList_[idx].dump();
    }
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

void Tx::onLoadFromEEPROM()
{
  uint8_t i;
  uint16_t addr = 0L;
  
  EEPROM.get(addr, i);
  if(i >= MAX_MODEL)    // EEPROM is corrupted
  {
    i = 0;
    error(ERR_EEPROM_DATA_CORRUPTED);
  }
  currentModel_ = &modelList_[i];
  addr += sizeof(uint8_t);
  
  for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    addr = modelList_[idx].getFromEEPROM(addr);
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    addr = sensor_[idx]->getFromEEPROM(addr);
}

void Tx::onSaveToEEPROM()
{
  uint8_t i = getCurrentModelIndex();
  uint16_t addr = 0L;
    
  EEPROM.put(addr, i);
  addr += sizeof(uint8_t);

  for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    addr = modelList_[idx].putToEEPROM(addr);
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    addr = sensor_[idx]->putToEEPROM(addr);
}

void Tx::onReset()
{
  // reset models 
  for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
    modelList_[idx].reset();
    
  // reset calibration
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    sensor_[idx]->reset();

  toggleMode_ = tTransmit;

  setupInputDevice();
}

uint8_t Tx::getCurrentModelIndex()
{
  for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
  {
    if(currentModel_ == &modelList_[idx])
      return idx;
  }
  return 0;
}

void Tx::onToggleSimulation()
{
  if(toggleMode_ == tTransmit)
  {
    error(ERR_DEBUG_FIRST);
    return;
  }
  
  toggleSimulation_ = !toggleSimulation_;
  mesure_.reset();

  if(toggleSimulation_)
  {
    info(INFO_SIMU_ON);
    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(true);
  }
  else
  {
    info(INFO_SIMU_OFF);
    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(false);
  }
}

void Tx::onSetSimulateSensorValue(uint8_t channel, uint16_t value)
{
  sensor_[channel]->setSimulateValue(value); 
}

