#include "Tx.h"
#include "FlashMem.h"
#include <EEPROM.h>

Tx::Tx()
:currentModel_(&modelList_[0]),
//ppmOut_(MAX_PPM_OUTPUT_CHANNEL, ppmOutputValue_, ppmWorkValue_, MAX_PPM_OUTPUT_CHANNEL),
toggleMode_(tTransmit),
toggleDisplayInputUpdate_(false),
toggleDisplayOutputUpdate_(false)
#ifdef GET_ADC_BY_IRQ
,adcIrqChannel_(0)
#endif
{
}

void Tx::setupInputSignal()
{
#ifdef GET_ADC_BY_IRQ
// Configure ADC for collecting data by interrupt way

  cli();
    
  // clear ADLAR to right-adjust the result ADCL will contain lower 8 bits, ADCH upper 2 bits
  ADMUX &= B11011111;
   
  // Set REFS1..0 to change reference voltage to the proper source (01)
  ADMUX |= B01000000;
  
  // Clear MUX3..0 in preparation for setting the analog input
  ADMUX &= B11110000;
  
  // Set MUX3..0 to read from A0 to A8
  //ADMUX |= B00000000;
  
  // Set ADEN to enable the ADC.
  // Note, this instruction takes 12 ADC clocks to execute
  ADCSRA |= B10000000;
  
  // Set ADATE to enable auto-triggering.
  //ADCSRA |= B00100000;
  
  // Clear ADTS2..0 to set trigger mode to free running.
  // This means that as soon as an ADC has finished, the next will be
  // immediately started.
  //ADCSRB &= B11111000;
  
  // Set the Prescaler to 128 le lowes possile (16000KHz/128 = 125KHz)
  // Above 200KHz 10-bit results are not reliable.
  ADCSRA |= B00000111;
  
  // Set ADIE to enable the ADC interrupt.
  // Without this, the internal interrupt will not trigger.
  ADCSRA |= B00001000;

  sei();
#endif
}


void Tx::setupOutputSignal()
{
#if 0
  Timer::setup();

  ppmOut_.setup();

  ppmOut_.setPulseLength(PPM_PULSE_LEN); // pulse length in microseconds
  ppmOut_.setPauseLength(PPM_PAUSE_LEN); // length of pause after last channel in microseconds

  ppmOut_.start(PPM_PIN, PPM_INVERT, PPM_DEBUG);
#endif

  digitalWrite(PPM_PIN, !PPM_SIGNAL);  //set the PPM signal pin to the default state
  
  cli();
  TCCR1A = 0;               // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;              // compare match register, change this
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

  setupInputSignal();

  // Serial setup() must always be the first module to initialize
  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

#ifdef GET_ADC_BY_IRQ
  // Kick off the first ADC to start the conversion
  ADCSRA |=B01000000;
#endif

  printf("Tx\t\tOK\n");
  return ret1 | ret2;
}

#ifdef GET_ADC_BY_IRQ
void Tx::onIrqAdcChange()
{
  // Warning this is an Interrupt routine

  // Get the ADC channel causing the interrupt
  int c = ADMUX & ((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

  // Get value, must read low first
  analogicSensorValueTab_[c] = ADCL | (ADCH << 8);

  // Select next channel
  ADMUX &= B11111000;
  adcIrqChannel_++;
  if(adcIrqChannel_ == 8)
    adcIrqChannel_ = 0;
  ADMUX |= adcIrqChannel_;
  
  // Set ADSC to start another conversion
  ADCSRA |= B01000000;
}
#endif


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
    static unsigned int calc_rest = 0;

    if(toggleMode_ == tTransmit)
      digitalWrite(PPM_PIN, !PPM_SIGNAL);
    state = true;

    if(currentChannelNumber >= MAX_PPM_OUTPUT_CHANNEL)
    {
      currentChannelNumber = 0;
      calc_rest += PPM_PULSE_LEN;
      OCR1A = (PPM_FRAME_LEN - calc_rest) * 2;
      calc_rest = 0;
    }
    else
    {
      OCR1A = (ppmOutputValue_[currentChannelNumber] - PPM_PULSE_LEN)*2;
      calc_rest += ppmOutputValue_[currentChannelNumber];
      currentChannelNumber++;
    }     
  }  
}

void Tx::idle()
{
  calculatePPMOutput();
  
  serialLink_.idle();

  if(toggleDisplayInputUpdate_)
    displayInputUpdate();
  if(toggleDisplayOutputUpdate_)
    displayOutputUpdate();
}

void Tx::calculatePPMOutput()
{
    // Get analogic input from all sensors
#ifndef GET_ADC_BY_IRQ
  for(int idx=0; idx < MAX_ADC_INPUT_CHANNEL; idx++)
    analogicSensorInputValue_[idx] = analogRead(A0 + idx);
#endif

  for(int idx=0; idx < MAX_DIG_INPUT_CHANNEL; idx++)
    digitalSensorInputValue_[idx] = digitalRead(idx);

  // convert analog value to to microseconds
  for(int idx=0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
    ppmOutputValue_[idx] = currentModel_->getOutputValue(idx, analogicSensorInputValue_[idx]);

//  if(toggleMode_ == tTransmit)
//     ppmOut_.update();
}

void Tx::displayInputUpdate()
{
#ifdef GET_ADC_BY_IRQ
      Serial.print("<I\t");   // get input from Irq way
#else
      Serial.print("<S\t");   // det inpout from synchrone way
#endif

    for(int idx = 0; idx < MAX_ADC_INPUT_CHANNEL; idx++)
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

  for(int idx = 0; idx < MAX_PPM_OUTPUT_CHANNEL; idx++)
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

void Tx::onDumpModel(int idx)
{
  // debug("[d] dump model %d\n", idx); 
  if(idx < MAX_MODEL)
  {
    char c = (currentModel_ == &modelList_[idx])?'*':' ';
    printf("Dump model %d %c\n",idx, c);

    modelList_[idx].dump();
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onCalibrateAnalogicSensors()
{
  //debug("[d] enter calibrate sensors loop\n");
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
  //debug("[d] reset\n");
  for(int idx = 0; idx < MAX_MODEL; idx++)
    modelList_[idx].reset();
    
  toggleMode_ = tTransmit;
}

uint8_t Tx::getCurrentModelIndex()
{
  for(int idx = 0; idx < MAX_MODEL; idx++)
  {
    if(currentModel_ == &modelList_[idx])
      return idx;
  }
  return 0;
}

