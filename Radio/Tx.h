#ifndef TX_H
#define TX_H

#include <arduino.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"
#include "Timer.h"
#include "Ppm.h"

class Tx
{
  SerialLink serialLink_;
  Command command_;
  Model modelList_[MAX_MODEL];
  Model *currentModel_;

  volatile int analogicSensorInputValue_[MAX_ADC_INPUT_CHANNEL];
  bool digitalSensorInputValue_[MAX_DIG_INPUT_CHANNEL];
  uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];
  uint8_t  ppmWorkValue_[PPM_WORK_SIZE(MAX_PPM_OUTPUT_CHANNEL)];
  Ppm ppmOut_;
    
  enum {tTransmit, tSetting} toggleMode_;
  bool toggleDisplayInputUpdate_;
  bool toggleDisplayOutputUpdate_;

  
#ifdef GET_ADC_BY_IRQ
  volatile byte adcIrqChannel_;
#endif

  // private functions
  void displayInputUpdate();
  void displayOutputUpdate();
  void setupOutputSignal();
  void setupInputSignal();
  
  public:
  
  Tx();
  
  bool setup();

  // Signals
#ifdef GET_ADC_BY_IRQ
  void onIrqAdcChange();
#endif
  void onChangeCurrentModel(int idx);
  void onDumpModel(int idx);
  void onToggleMode();
  void onToggleDisplayInputUpdate();
  void onToggleDisplayOutputUpdate();
  void onCalibrateAnalogicSensors();
  void onLoadFromEEPROM();
  void onSaveToEEPROM();
  void onReset();

  // Functions
  void idle();
  void calculatePPMOutput();
  Model* getCurrentModel() {return currentModel_;}
  uint8_t getCurrentModelIndex();
};

#endif
