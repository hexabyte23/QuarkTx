#include "Tx.h"
#include "FlashMem.h"
#include "Mesure.h"
#include <EEPROM.h>
#include <SPI.h>
//#include <SD.h>

static Mesure mesure;

Tx::Tx()
:currentModel_(&modelList_[0]),
toggleMode_(tTransmit),
ledState_(LOW),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false),
toggleCalibrateSensor_(false),
BTSerie_(BT_RX_PIN, BT_TX_PIN)
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
  s1_.setup(2);
  s2_.setup(3);
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
 
  sei();

  // LED
  pinMode(LED_PIN, OUTPUT);

  // Check BT module
  pinMode(BT_RX_PIN, INPUT);  
  pinMode(BT_TX_PIN, OUTPUT);  
  BTSerie_.begin(9600);
}

bool Tx::setup()
{
  mesure.start();
  
  // Extend battery duration
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

  rl_.setup(inputValue_, ppmOutputValue_, currentModel_);
  
  // Setup Timer for PPM signal generation
  setupOutputDevice();

  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

  onLoadFromEEPROM();
  
  mesure.stop();
  
  info(INFO_TX_READY,mesure.getAverage());
  return ret1 | ret2;
}

void Tx::onIrqTimerChange()
{
  static boolean state = true;
  
  TCNT1 = 0;
  
  if(state) 
  {  
    // Start pulse
    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, PPM_SIGNAL);
      
    OCR1A = PPM_PULSE_LEN*2;
    state = false;
  }
  else
  {
    // End pulse and calculate when to start the next pulse
    static byte currentChannelNumber = 0;
    static uint16_t remainingTime = 0;

    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, !PPM_SIGNAL);
    state = true;

    if(currentChannelNumber >= MAX_PPM_OUTPUT_CHANNEL)
    {
      currentChannelNumber = 0;
      remainingTime += PPM_PULSE_LEN;
      OCR1A = (PPM_FRAME_LEN - remainingTime)*2;
      remainingTime = 0;
    }
    else
    {
      OCR1A = (ppmOutputValue_[currentChannelNumber] - PPM_PULSE_LEN)*2;
      remainingTime += ppmOutputValue_[currentChannelNumber];
      currentChannelNumber++;
    }     
  }  
}

void Tx::idle()
{
  calculatePPMOutputIdle();
  rl_.idle();
  ledBlinkIdle();
  serialLink_.idle();

  if(toggleDisplayInputUpdate_)
    displayInputUpdate();
  if(toggleDisplayOutputUpdate_)
    displayOutputUpdate();
  if(toggleCalibrateSensor_)
    calibrateSensor();
}

/*
void Tx::calculatePPMOutputIdle()
{
  mesure.p1();
   // Get analogic input sensors
  for(uint8_t idx=0; idx < MAX_ADC_INPUT_CHANNEL; idx++)
    inputValue_[idx] = analogRead(A0 + idx);

  // Get digital input sensors
  uint8_t digIdx=0;
  for(uint8_t idx=MAX_ADC_INPUT_CHANNEL; idx < MAX_INPUT_CHANNEL; idx++, digIdx++)
    inputValue_[idx] = digitalRead(digMapping_[digIdx])==HIGH?ADC_MAX_VALUE:ADC_MIN_VALUE;
  // 456 462 568

  mesure.p2();
  mesure.displayAvg(500);
  // Convert analog values to microseconds
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
  {
    ppmOutputValue_[idx] = currentModel_->getValue(idx, inputCalibrMin_[idx], 
                                                        inputCalibrMax_[idx], 
                                                        inputValue_[idx]);
  }
  // 400 408 448

  // 856 868 964
}
*/

void Tx::calculatePPMOutputIdle()
{
//  mesure.p1();
   // Get input sensors values
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
    inputValue_[idx] = sensor_[idx]->getValue();
  // 836 846 932
  
  // Convert analog values to microseconds
//  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
//    ppmOutputValue_[idx] = currentModel_->getValue(idx, inputValue_[idx]);
  //360 369 408

//  mesure.p2();
//  mesure.displayAvg(500);
  // 1196 1212 1300
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
    Serial.print(inputValue_[idx], DISPLAY_BASE);
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
     toggleMode_ = tSetting;
     info(INFO_SWITCH_MODE_SETTINGS);
  }
  else
  {
     toggleMode_ = tTransmit;
     info(INFO_SWITCH_MODE_TRANSMIT);
  }
}

void Tx::onChangeCurrentModel(int idx)
{
  //debug("[d] load model %d\n", idx);
  if(idx < MAX_MODEL)
  {
    info(INFO_LOAD_MODEL,idx);
    currentModel_ = &modelList_[idx];
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onDump()
{
  // dump calibrate
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

void Tx::onToggleCalibrateSensor()
{
  //debug("[d] enter calibrate sensors loop\n");
  toggleCalibrateSensor_ = !toggleCalibrateSensor_;
}

/*
void Tx::calibrateSensor(bool displayOnly)
{  
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    if(displayOnly == false)
    {
      if(inputCalibrMin_[idx] > inputValue_[idx])
        inputCalibrMin_[idx] = inputValue_[idx];
      if(inputCalibrMax_[idx] < inputValue_[idx])
        inputCalibrMax_[idx] = inputValue_[idx];
    }
      
    Serial.print("{");
    Serial.print(inputCalibrMin_[idx], DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(inputCalibrMax_[idx], DISPLAY_BASE);
    Serial.print("}\t");
  }
  Serial.println();
}
*/
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
  
  EEPROM.get(0, i);
  if(i >= MAX_MODEL)    // EEPROM is corrupted
  {
    i = 0;
    error(ERR_EEPROM_DATA_CORRUPTED);
  }
  currentModel_ = &modelList_[i];
  //debug("load %d\n", i);
  EEPROM.get(sizeof(uint8_t), modelList_);
}

void Tx::onSaveToEEPROM()
{
  uint8_t i = getCurrentModelIndex();
  //debug("save %d\n", i);
  EEPROM.put(0, i);
  EEPROM.put(sizeof(uint8_t), modelList_);
}

void Tx::onReset()
{
  // reset models 
  for(uint8_t idx = 0; idx < MAX_MODEL; idx++)
    modelList_[idx].reset();
  /*
  // reset calibration
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  { 
    inputCalibrMin_[idx] = 0xFFFF;
    inputCalibrMax_[idx] = 0x0;
  }
  */
  toggleMode_ = tTransmit;
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


