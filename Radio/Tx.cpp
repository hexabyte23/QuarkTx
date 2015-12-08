#include "Tx.h"
#include "FlashMem.h"

Tx::Tx()
:currentModel_(&modelList_[0]),
ppmOutputRef_(modelList_[0].getPPMOutputRef()),
updateToSerial_(false)
#ifdef GET_ADC_BY_IRQ
,adcIrqChannel_(0)
#endif
{
  // set ref to each models
  for(int idx=0; idx < MAX_MODEL; idx++)
    modelList_[idx].setup(analogicSensorValueTab_);
}

#ifdef GET_ADC_BY_IRQ
void Tx::setupIrqADC()
{
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
}
#endif

void Tx::setupIrqPPM()
{
/*  
  TCCR1A = B00110001; // COM1B1, COM1B0, WGM10 set to 1 (8-bit register)
  TCCR1B = B00010010; // WGM13 & CS11 set to 1 (8-bit register)
  TCCR1C = B00000000;
  TIMSK1 = B00000010; // All interrupts are individually masked with the Timer Interrupt Mask Register TIMSK1
  TIFR1  = B00000010; // Int on compare B

//  OCR1A = PPM_PERIOD; // PPM frequency (double buffered Output Compare 16-bit Register)
//  OCR1B = PPM_LOW;    // (double buffered Output Compare 16-bit Register), hard-wired to PPM_PIN
*/

  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
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
  pinMode(A4, INPUT_PULLUP);    // reserved for extra POT
  pinMode(A5, INPUT_PULLUP);    // reserved for extra POT
  pinMode(A7, INPUT);           // Battery level

  // Set the PPM signal pin to the default state (off)
  digitalWrite(PPM_PIN, !PPM_POLARITY);
  
  cli();

  // Configure Timer1 for PPM signal generation
  setupIrqPPM();

#ifdef GET_ADC_BY_IRQ
  // Configure ADC for collecting data by interrupt way
  setupIrqADC();
#endif
  
  sei();

  // Serial setup() must always be the first module to initialize
  bool ret1 = serialLink_.setup(&command_);
  bool ret2 = command_.setup(this);

#ifdef GET_ADC_BY_IRQ
  // Kick off the first ADC to start the conversion
  ADCSRA |=B01000000;
#endif

  printf("Tx\t\tOK\n");

  return true;
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

#else

void Tx::syncAdcUpdate()
{
  for(int idx=0; idx < MAX_ADC_CHANNEL; idx++)
  {
    analogicSensorValueTab_[idx] = analogRead(idx);
  }
}

#endif

static boolean state = true;
void Tx::onIrqTimerChange()
{
  TCNT1 = 0;
  
  if(state) 
  {  
    // Start pulse
    digitalWrite(PPM_PIN, PPM_POLARITY);
    OCR1A = PPM_PULSE_LEN*2;
    state = false;
  }
  else
  {
    // End pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(PPM_PIN, !PPM_POLARITY);
    state = true;

    if(cur_chan_numb >= MAX_PPM_CHANNEL)
    {
      cur_chan_numb = 0;
      calc_rest += PPM_PULSE_LEN;
      OCR1A = (PPM_FRAME_LEN - calc_rest) * 2;
      calc_rest = 0;
    }
    else
    {
      OCR1A = (*(ppmOutputRef_ + cur_chan_numb) - PPM_PULSE_LEN)*2;
      calc_rest += *(ppmOutputRef_ + cur_chan_numb);
      cur_chan_numb++;
    }     
  }
  
}

void Tx::idle()
{
  currentModel_->idle();
  serialLink_.idle();

#ifndef GET_ADC_BY_IRQ
  syncAdcUpdate();
#endif

  if(updateToSerial_)
  {
#ifdef GET_ADC_BY_IRQ
      Serial.print("I\t");
#else
      Serial.print("R\t");
#endif

    for(int idx = 0; idx < MAX_ADC_CHANNEL; idx++)
    {
      Serial.print(analogicSensorValueTab_[idx], HEX);
      Serial.print("\t");
    }
    Serial.println();
    
  }
}

void Tx::onChangeCurrentModel(int idx)
{
  //debug("[d] load model %d\n", idx);
  if(idx < MAX_MODEL)
  {
    printf("Load model %d\n",idx);
    currentModel_ = &modelList_[idx];
    ppmOutputRef_ = currentModel_->getPPMOutputRef();
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onDumpModel(int idx)
{
  // debug("[d] dump model %d\n", idx); 
  if(idx < MAX_MODEL)
  {
    printf("Dump model %d\n",idx);
    modelList_[idx].dump();
  }
  else
    error(ERR_BAD_PARAM_IDX_HIGH, idx, MAX_MODEL);
}

void Tx::onCalibrateAnalogicSensors()
{
  debug("[d] enter calibrate sensors loop\n");
}


