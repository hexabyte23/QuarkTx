#include "Tx.h"
#include "FlashMem.h"
#include <EEPROM.h>

Tx::Tx()
:currentModel_(&modelList_[0]),
toggleMode_(tTransmit),
ledState_(LOW),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false),
toggleDisplayCalibrate_(false)
#ifdef GET_ADC_BY_IRQ
,adcIrqChannel_(0)
#endif
{
  onReset();
}


void Tx::setupOutputSignal()
{
  digitalWrite(PPM_PIN, !PPM_SIGNAL);  //set the PPM signal pin to the default state
  
  cli();
  
  TCCR1A = 0;               // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;              // compare match register, initial value

  TCCR1B |= (1 << WGM12);   // turn on CTC mode
  TCCR1B |= (1 << CS11);    // 8 prescaler: 0,5 microseconds at 16mhz
 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
 
  sei();
}

bool Tx::setup()
{
  // Digital pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(PPM_PIN, OUTPUT);     // PPM
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);     // Led

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
  
  // Configure Timer for PPM signal generation
  setupOutputSignal();

  // Serial setup() must always be the first module to initialize
  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

  onLoadFromEEPROM();
  
  printf("Tx\t\tOK\n");
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
  ledBlink();
  serialLink_.idle();

  if(toggleDisplayInputUpdate_)
    displayInputUpdate();
  if(toggleDisplayOutputUpdate_)
    displayOutputUpdate();
  if(toggleDisplayCalibrate_)
    displayCalibrate(false);
}

void Tx::calculatePPMOutputIdle()
{
   // Get analogic input sensors
  for(uint8_t idx=0; idx < MAX_ADC_INPUT_CHANNEL; idx++)
    analogicSensorInputValue_[idx] = analogRead(A0 + idx);

  // Get digital input sensors
  uint8_t digIdx=0;
  for(uint8_t idx=MAX_ADC_INPUT_CHANNEL; idx < MAX_INPUT_CHANNEL; idx++, digIdx++)
    analogicSensorInputValue_[idx] = digitalRead(digMapping_[digIdx])==HIGH?1023:0;

  // Convert analog value to to microseconds
  for(uint8_t idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    ppmOutputValue_[idx] = currentModel_->getValue(idx, analogicSensorCalibMin_[idx], 
                                                        analogicSensorCalibMax_[idx], 
                                                        analogicSensorInputValue_[idx]);
}

void Tx::ledBlink()
{
  if(toggleMode_ == tTransmit)
  {
    unsigned long currentMillis = millis();
  
    if(currentMillis - previousMillis_ >= LED_BLINK_PERIOD) 
    {
      previousMillis_ = currentMillis;
  
      if (ledState_ == LOW) 
        ledState_ = HIGH;
      else
        ledState_ = LOW;
      
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
    Serial.print(analogicSensorInputValue_[idx], DISPLAY_BASE);
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
    printf("Load model %d\n",idx);
    currentModel_ = &modelList_[idx];
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onDump()
{
  // dump calibrate
  info(INFO_CALIBRATE, MAX_INPUT_CHANNEL);
  displayCalibrate(true);
  
  // dump models
  for(uint8_t idx=0; idx < MAX_MODEL; idx++)
  {
    char c = (currentModel_ == &modelList_[idx])?'*':' ';
    printf("Model %d %c\n",idx, c);
  
    modelList_[idx].dump();
  }
}

void Tx::onToggleCalibrateAnalogicSensors()
{
  //debug("[d] enter calibrate sensors loop\n");
  toggleDisplayCalibrate_ = !toggleDisplayCalibrate_;
}

void Tx::displayCalibrate(bool displayOnly)
{  
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  {
    if(displayOnly == false)
    {
      if(analogicSensorCalibMin_[idx] > analogicSensorInputValue_[idx])
        analogicSensorCalibMin_[idx] = analogicSensorInputValue_[idx];
      if(analogicSensorCalibMax_[idx] < analogicSensorInputValue_[idx])
        analogicSensorCalibMax_[idx] = analogicSensorInputValue_[idx];
    }
      
    Serial.print("{");
    Serial.print(analogicSensorCalibMin_[idx], DISPLAY_BASE);
    Serial.print("\t");
    Serial.print(analogicSensorCalibMax_[idx], DISPLAY_BASE);
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

  // reset calibration
  for(uint8_t idx=0; idx < MAX_INPUT_CHANNEL; idx++)
  { 
    analogicSensorCalibMin_[idx] = 0xFFFF;
    analogicSensorCalibMax_[idx] = 0x0;
  }
  
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

