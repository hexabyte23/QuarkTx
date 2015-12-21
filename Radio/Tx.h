#ifndef TX_H
#define TX_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"
#include "Sensor.h"
#include "Rl.h"


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
  uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];

  // mixers, dual rate, expo...
  Rl radioLanguage_;

  // LED
  int ledState_;
  unsigned long ledPrevMS_;

  // BT
  //SoftwareSerial BTSerie_;

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
  void ledBlinkIdle();
  
  public:
  
  Tx();
  
  bool setup();

  // Signals
  void onIrqTimerChange();
  void onChangeCurrentModel(int idx);
  void onDump(const char* param);
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
