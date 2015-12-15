#ifndef TX_H
#define TX_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"
#include "Sensor.h"


class Tx
{
  SerialLink serialLink_;
  Command command_;
  Model modelList_[MAX_MODEL];
  Model *currentModel_;

  // Input & output datas
  Stick elevator_, aileron_, rudder_, throttle_;
  Switch s1_, s2_;
  BatteryMeter battery_;
  Sensor *sensor_[MAX_INPUT_CHANNEL] = { &elevator_, &aileron_, &rudder_, &throttle_, &s1_, &s2_};
  uint16_t inputValue_[MAX_INPUT_CHANNEL];
  uint16_t inputCalibrMin_[MAX_INPUT_CHANNEL];
  uint16_t inputCalibrMax_[MAX_INPUT_CHANNEL];
  uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];

  // Channels mapping for switches
  uint8_t digMapping_[MAX_DIG_INPUT_CHANNEL] = {2, 3};
  

  // LED
  int ledState_;
  unsigned long ledPrevMS_;

  // BT
  SoftwareSerial BTSerie_;

  // toggles
  enum {tTransmit, tSetting} toggleMode_;
  bool toggleDisplayInputUpdate_;
  bool toggleDisplayOutputUpdate_;
  bool toggleCalibrateSensor_;


  // private functions
  void displayInputUpdate();
  void displayOutputUpdate();
  void setupInputDevice();
  void setupOutputDevice();
  void calibrateSensor();
  void calculatePPMOutputIdle();
  void ledBlink();
  
  public:
  
  Tx();
  
  bool setup();

  // Signals
  void onIrqTimerChange();
  void onChangeCurrentModel(int idx);
  void onDump();
  void onToggleMode();
  void onToggleDisplayInputUpdate();
  void onToggleDisplayOutputUpdate();
  void onToggleCalibrateSensor();
  void onLoadFromEEPROM();
  void onSaveToEEPROM();
  void onReset();

  // Functions
  void idle();
  Model* getCurrentModel() const {return currentModel_;}
  uint8_t getCurrentModelIndex();
};

#endif
