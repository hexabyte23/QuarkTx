#ifndef TX_H
#define TX_H

#include <arduino.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"

class Tx
{
  SerialLink serialLink_;
  Command command_;
  Model modelList_[MAX_MODEL];
  Model *currentModel_;
  bool updateToSerial_;
  int *ppmOutputRef_;
  
#ifdef GET_ADC_BY_IRQ
  volatile byte adcIrqChannel_;
#endif
  volatile int analogicSensorValueTab_[MAX_ADC_CHANNEL];

  // private functions
  void setupIrqPPM();
#ifdef GET_ADC_BY_IRQ
  void setupIrqADC();
#else
  void syncAdcUpdate();
#endif
  
  public:
  
  Tx();
  
  bool setup();

  // Signals
#ifdef GET_ADC_BY_IRQ
  void onIrqAdcChange();
#endif
  void onIrqTimerChange();
  void onChangeCurrentModel(int idx);
  void onDumpModel(int idx);
  void onStartUpdateToSerial() {updateToSerial_= true;}
  void onStopUpdateToSerial()  {updateToSerial_= false;}
  void onCalibrateAnalogicSensors();

  // Functions
  void idle();
};

#endif
