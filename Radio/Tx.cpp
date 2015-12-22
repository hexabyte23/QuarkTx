#include "Tx.h"
#include "FlashMem.h"
#include <EEPROM.h>
#include <SPI.h>
//#include <SD.h>


Tx::Tx()
:currentModel_(&modelList_[0]),
toggleMode_(tTransmit),
ledState_(LOW),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false),
toggleCalibrateSensor_(false),
toggleSimulation_(false)
//BTSerie_(BT_RX_PIN, BT_TX_PIN)
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
  // PPM
  pinMode(PPM_PIN, OUTPUT);
  digitalWrite(PPM_PIN, !PPM_SIGNAL);  //set the PPM signal pin to the default state
  
  cli();
  
  TCCR1A = 0;               // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;              // compare match register, initial value

  TCCR1B |= (1 << WGM12);   // turn on CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler: 0,5 microseconds at 16mhz
 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  // init irq variables
  irqState_ = true;
  irqCurrentChannelNumber_ = 0;
  irqRemainingTime_ = 0;
  
  sei();

  // LED
  pinMode(LED_PIN, OUTPUT);

  // Check BT module
//  pinMode(BT_RX_PIN, INPUT);  
//  pinMode(BT_TX_PIN, OUTPUT);  
//  BTSerie_.begin(9600);
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
  pinMode(A4, INPUT);           
  digitalWrite(A4, HIGH);       // reserved for extra POT
  pinMode(A5, INPUT);           
  digitalWrite(A5, HIGH);       // reserved for extra POT
  pinMode(A7, INPUT);           // Battery level

  // Setup input sensors
  setupInputDevice();

  evaluator_.setup(sensor_, ppmOutputValue_, currentModel_);
  
  // Setup Timer for PPM signal generation
  setupOutputDevice();

  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

  onLoadFromEEPROM();
  
  mesure_.stop();
  info(INFO_TX_READY,mesure_.getAverage());
  
  return ret1 | ret2;
}

void Tx::onIrqTimerChange()
{  
  TCNT1 = 0;
  
  if(irqState_) 
  {  
    // Start pulse
    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, PPM_SIGNAL);
      
    OCR1A = PPM_PULSE_LEN*2;
    irqState_ = false;
  }
  else
  {
    // End pulse, calculate when to start the next pulse
    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, !PPM_SIGNAL);
    irqState_ = true;

    if(irqCurrentChannelNumber_ >= MAX_PPM_OUTPUT_CHANNEL)
    {
      irqCurrentChannelNumber_ = 0;
      irqRemainingTime_ += PPM_PULSE_LEN;
      OCR1A = (PPM_FRAME_LEN - irqRemainingTime_)*2;
      irqRemainingTime_ = 0;
    }
    else
    {
      OCR1A = (ppmOutputValue_[irqCurrentChannelNumber_] - PPM_PULSE_LEN)*2;
      irqRemainingTime_ += ppmOutputValue_[irqCurrentChannelNumber_];
      irqCurrentChannelNumber_++;
    }     
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
    mesure_.displayAvg(1000);
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
  Serial.print("<S\t");   // det inpout from synchrone way

  for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    Serial.print(sensor_[idx]->getValue(), DISPLAY_BASE);
    Serial.print("\t");
  }
  Serial.println();
}

void Tx::onToggleDisplayInputUpdate()
{
  toggleDisplayInputUpdate_ = !toggleDisplayInputUpdate_;
}

void Tx::displayOutputUpdate()
{
  Serial.print(">\t");

  for(uint8_t idx = 0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    Serial.print((toggleMode_ == tTransmit)?ppmOutputValue_[idx]:0, DISPLAY_BASE);
    Serial.print("\t");
  }

  Serial.println();
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
      {
        Serial.print(idx, HEX);
        Serial.print("\t");
      }
      
      Serial.print(EEPROM.read(idx), HEX);
      Serial.print(" ");
    
      if(i == 15)
      {
        i = -1;
        Serial.println();
      }
    }
    Serial.println();
  }
  else
  {
    // dump sensors
    info(INFO_SENSOR, MAX_INPUT_CHANNEL);
    //calibrateSensor(true);
    for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    {
      Serial.print(idx, DISPLAY_BASE);
      Serial.print(" ");
      sensor_[idx]->dump();
      Serial.println();
    }
    Serial.println();
    
    // dump models
    for(uint8_t idx=0; idx < MAX_MODEL; idx++)
    {
      char c = (currentModel_ == &modelList_[idx])?'*':' ';
      printf("Model %d %c\n",idx, c);
    
      modelList_[idx].dump();
    }
  }
}

void Tx::onToggleCalibrateSensor()
{
  //debug("[d] enter calibrate sensors loop\n");
  toggleCalibrateSensor_ = !toggleCalibrateSensor_;
}

void Tx::calibrateSensor()
{  
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    sensor_[idx]->calibrate();
    Serial.print("{");
    Serial.print(sensor_[idx]->getMinCalibration(), DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(sensor_[idx]->getMaxCalibration(), DISPLAY_BASE);
    Serial.print("}\t");
  }
  Serial.println();
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
  
  //EEPROM.get(addr, modelList_);
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
  //EEPROM.put(addr, modelList_);
  //addr += sizeof(modelList_);
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
    printf("[e] failed, switch in debug mode first\n");
    return;
  }
  
  toggleSimulation_ = !toggleSimulation_;
  mesure_.reset();

  if(toggleSimulation_)
  {
    printf("Simulation on\n");
    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(true);
  }
  else
  {
    printf("Simulation off\n");
    for(uint8_t idx = 0; idx < MAX_INPUT_CHANNEL; idx++)
      sensor_[idx]->setSimulation(false);
  }
}

void Tx::onSetSimulateSensorValue(uint8_t channel, uint16_t value)
{
  debug("simu %d %d\n", channel, value);

  sensor_[channel]->setSimulateValue(value); 
}

