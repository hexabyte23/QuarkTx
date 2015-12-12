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

  // Input & output datas
  volatile uint16_t analogicSensorInputValue_[MAX_ADC_INPUT_CHANNEL];
  volatile uint16_t analogicSensorCalibMin_[MAX_ADC_INPUT_CHANNEL];
  volatile uint16_t analogicSensorCalibMax_[MAX_ADC_INPUT_CHANNEL];
  bool digitalSensorInputValue_[MAX_DIG_INPUT_CHANNEL];
  uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];

  // toggles
  enum {tTransmit, tSetting} toggleMode_;
  bool toggleDisplayInputUpdate_;
  bool toggleDisplayOutputUpdate_;
  bool toggleDisplayCalibrate_;

  
#ifdef GET_ADC_BY_IRQ
  volatile byte adcIrqChannel_;
#endif

  // private functions
  void displayInputUpdate();
  void displayOutputUpdate();
  void setupOutputSignal();
  void setupInputSignal();
  void displayCalibrate();
  
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
  void onToggleMode();
  void onToggleDisplayInputUpdate();
  void onToggleDisplayOutputUpdate();
  void onToggleCalibrateAnalogicSensors();
  void onLoadFromEEPROM();
  void onSaveToEEPROM();
  void onReset();

  // Functions
  void idle();
  void calculatePPMOutput();
  Model* getCurrentModel() const {return currentModel_;}
  uint8_t getCurrentModelIndex();
};

#endif
