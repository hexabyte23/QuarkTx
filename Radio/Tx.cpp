/*
Tx.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Betrand WILMOT.  All rights reserved.

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


Tx::Tx()
:currentModel_(&modelList_[0]),
toggleTxMode_(tTransmit),
ledState_(LOW),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false),
toggleCalibrateSensor_(false),
toggleSimulation_(false)
{
  onSoftwareReset("");
}

void Tx::setupInputDevice()
{
  //const unsigned char PS_16 = (1 << ADPS2);                                 // 1 MHz
  //const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);                  // 500 KHz
  //const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);                  // 250 KHz
  //const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // 125 KHz

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
  TCCR1B |= (1 << CS11);    // prescaler to 8 -> 0.5 microseconds at 16mhz
 
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
  
  return ret1 | ret2;
}

void Tx::onIrqTimerChange()
{
  /*
   * With new 2.4 GHz HF modules, we dont care anymore about 20 ms constraint 
   * (as HF modules can now sent more than 8 channels) but just care about 2 times:
   * The minimum time between 2 channels pulses (PPM_INTER_CHANNEL_TIME) 
   * The minimum time between 2 PPM frames (PPM_INTER_FRAME_TIME)
   * 
   * We successfully test with a Jeti TU2 RF module up to 17 channels
   */
 
  TCNT1 = 0;

  if(irqStartPulse_) 
  {  
    // Falling edge of a channel pulse (in negative shape)
    if(toggleTxMode_ == tTransmit)
      digitalWrite(PPM_PIN, !PPM_SHAPE_SIGNAL);

    // All time must be x2, as prescale is set to 0.5 microseconds at 16mhz
    OCR1A = ppmOutputValue_[irqCurrentChannelNumber_]*2;
    irqCurrentChannelNumber_++;
    irqStartPulse_ = false;
  }
  else
  {
    // Raising edge of a channel pulse (in negative shape)
    if(toggleTxMode_ == tTransmit)
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

void Tx::displayInputUpdate()
{
  STDOUT << F("<\t");
  
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
  STDOUT << F(">\t");

  for(uint8_t idx = 0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    STDOUT << ppmOutputValue_[idx] << "\t";

  STDOUT << endl;
}

void Tx::onToggleDisplayOutputUpdate()
{
  toggleDisplayOutputUpdate_ = !toggleDisplayOutputUpdate_;
}

void Tx::onToggleTxMode()
{
  if(toggleTxMode_ == tTransmit)
  {
    toggleTxMode_ = tDebug;
    STDOUT << F("Mode switched to 'debug'") << endl;
    
    mesure_.reset();
  }
  else
  {
    toggleTxMode_ = tTransmit;
    STDOUT << F("Mode switched to 'transmit'") << endl;
    
    mesure_.reset();
  }
}

void Tx::onChangeCurrentModel(int idx)
{
  if(idx < MAX_MODEL)
  {
    STDOUT << F("Load model ") << idx << endl;
    currentModel_ = &modelList_[idx];
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
  STDOUT << F("Sensors (") << MAX_INPUT_CHANNEL << F(")\n# Pin   Trim    Min     Max    Simu") << endl;

  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    STDOUT << idx << " ";
    sensor_[idx]->dump();
    STDOUT << endl;
  }
  STDOUT << endl; 
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

void Tx::onLoadFromEEPROM()
{
  uint8_t i;
  uint16_t addr = 0L;

  // get current model used index
  EEPROM.get(addr, i);
  if(i >= MAX_MODEL)    // EEPROM is corrupted
  {
    i = 0;
    STDOUT << F("e-edic") << endl;    //  EEPROM data is corrupted
    return;
  }
  currentModel_ = &modelList_[i];
  addr += sizeof(uint8_t);

  // get Model data
  for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    addr = modelList_[idx].loadFromEEPROM(addr);

  // get Sensors data
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    addr = sensor_[idx]->loadFromEEPROM(addr);
  
  rcl_.loadFromEEPROM(addr);
}

void Tx::onSaveToEEPROM()
{

  uint8_t i = getModelIndex(currentModel_);
  uint16_t addr = 0L;
/*
  // save current model index
  EEPROM.put(addr, i);
  addr += sizeof(uint8_t);

  // save Model data
  for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    addr = modelList_[idx].saveToEEPROM(addr);

  // save Sensor data
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    addr = sensor_[idx]->saveToEEPROM(addr);
*/
  rcl_.saveToEEPROM(addr);
}

void Tx::resetModel()
{
  for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
    modelList_[idx].reset();
}

void Tx::resetSensor()
{
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    sensor_[idx]->reset();
}

void Tx::resetRCL()
{
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    rcl_.clearRCL(idx);
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

void Tx::onToggleSimulation()
{
  if(toggleTxMode_ == tTransmit)
  {
    STDOUT << F("e-sdf") << endl;    //  switch to simulation failed, switch in debug mode first
    return;
  }
  
  toggleSimulation_ = !toggleSimulation_;
  mesure_.reset();

  if(toggleSimulation_)
  {
    STDOUT << F("Simulation ON") << endl;
    
    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(true);
  }
  else
  {
    STDOUT << F("Simulation OFF") << endl;

    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(false);
  }
}

void Tx::onSetSimulateSensorValue(uint8_t sensorID, uint16_t value)
{
  if(sensorID < 0) return;
  sensor_[sensorID]->setSimulateValue(value); 
}

void Tx::onSetTrimSensorValue(uint8_t sensorID, int value)
{
  if(sensorID < 0) return;
  sensor_[sensorID]->setTrim(value); 
}

void Tx::onSetRCL(uint8_t chan, const char* rclCode)
{
  if(chan < 0) return;
  if(rclCode[0] == 0)
  {
    STDOUT << chan << F(" cleaned") << endl;
    rcl_.clearRCL(chan);
  }
  else
  {
    STDOUT << chan << " " << rclCode << endl;
    rcl_.setupRCL(chan, rclCode); 
  }
}

