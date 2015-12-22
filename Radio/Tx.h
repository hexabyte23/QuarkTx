#ifndef TX_H
#define TX_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"
#include "Sensor.h"
#include "Evaluator.h"
#include "Mesure.h"


class Tx
{
  SerialLink serialLink_;
  Command command_;
  Model modelList_[MAX_MODEL];
  Model *currentModel_;

  // Input & output datas
  Stick elevator_, aileron_, rudder_, throttle_;
  BatteryMeter battery_;
#ifdef TERRATOP
  Switch s1_, s2_, s3_;
  Sensor *sensor_[MAX_INPUT_CHANNEL] = { &elevator_, &aileron_, &rudder_, &throttle_, &s1_, &s2_, &s3_};
#else
  Switch s1_, s2_;
  Sensor *sensor_[MAX_INPUT_CHANNEL] = { &elevator_, &aileron_, &rudder_, &throttle_, &s1_, &s2_};
#endif
  uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];

  // mixers, dual rate, expo...
  Evaluator evaluator_;

  Mesure mesure_;

  // LED
  int ledState_;
  unsigned long ledPrevMS_;

  // BT
  //SoftwareSerial BTSerie_;

  // toggles
  enum {tTransmit, tDebug} toggleMode_;
  bool toggleDisplayInputUpdate_;
  bool toggleDisplayOutputUpdate_;
  bool toggleCalibrateSensor_;
  bool toggleSimulation_;

  // Private functions
  void displayInputUpdate();
  void displayOutputUpdate();
  void setupInputDevice();
  void setupOutputDevice();
  void calibrateSensor();
  void ledBlinkIdle();

  // Irq
  volatile boolean irqState_;
  volatile byte irqCurrentChannelNumber_;
  volatile uint16_t irqRemainingTime_;
  
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
  void onToggleSimulation();
  void onLoadFromEEPROM();
  void onSaveToEEPROM();
  void onReset();
  void onSetSimulateSensorValue(uint8_t chan, uint16_t value);

  // Functions
  void idle();
  Model* getCurrentModel() const {return currentModel_;}
  uint8_t getCurrentModelIndex();
};

#endif
