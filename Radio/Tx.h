/*
Tx.h - QuarkTx
Copyright (c) 2015-2018 Thierry & Bertrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TX_H
#define TX_H

#include <arduino.h>
#include "config.h"
#include "SerialLink.h"
#include "Command.h"
#include "Model.h"
#include "Sensor.h"
#include "RCLEval.h"
#include "Mesure.h"
#include "Fs.h"


class Tx
{
   SerialLink serialLink_;
   Command command_;
   Model modelList_[QUARKTX_MAX_MODEL];
   Model *currentModel_;
   BatteryMeter battMeter_;
   Sensor *sensor_[QUARKTX_MAX_INPUT_CHANNEL];
   volatile uint16_t ppmOutputValue_[QUARKTX_MAX_PPM_OUTPUT_CHANNEL];
   unsigned int displayInputPeriod_, displayInputPrevMs_, displayOutputPeriod_, displayOutputPrevMs_;
   unsigned long currentMs_;
#ifdef QUARKTX_TEENSY
   QuarkTxFileSystem fs_;
#endif

   // For mixers, dual rate, expo...
   RCLEval rcl_;

   Mesure mesure_;

   // Booting seq
   bool isBootSeqAlreadyDisplayed_, serialOk_, sdOk_, sdLoad_;
   void displayBootingSequence();

   // Battery level chack
   int battPrevMs_;
   void battLevelCheck();

   // LED
   int ledState_;
   unsigned long ledPrevMs_, ledBlinkPeriod_;
   void ledBlinkUpdate();

   // toggles
   enum {tTransmit, tDebug} toggleTxMode_;
   bool toggleDisplayInputUpdate_;
   bool toggleDisplayOutputUpdate_;
   bool toggleCalibrateSensor_;

   // Private functions
   void displayInputUpdate();
   void displayOutputUpdate();
   void setupInputDevice();
   void setupOutputDevice();
   void calibrateSensor();
   void dumpEEPROM();
   void dumpModel();
   void dumpSensor();
   void dumpRCL(const char* param);
#ifdef QUARKTX_TEENSY
   void dumpDirectory(const char* param);
#endif
   void resetModel();
   void resetSensor();
   void resetRCL();

   // used for Isr
   volatile boolean isrStartPulse_;
   volatile byte isrCurrentChannelNumber_;

public:

   Tx();

   // Public signals
   void onIsrTimerChange();
   void onChangeCurrentModel(int idx);
   void onDump(const char* param);
   void onToggleTxMode();
   void onToggleDisplayInputUpdate(int period);
   void onToggleDisplayOutputUpdate(int period);
   void onToggleCalibrateSensor();
#ifdef QUARKTX_TEENSY
   bool onLoadFromFile();
   bool onSaveToFile();
#endif
   bool onLoadFromEEPROM();
   void onSaveToEEPROM();
   void onSoftwareReset(const char* param);
   void onSetTrimSensorValue(uint8_t sensorID, int value);
   void onSetTrimBattery(int value);
   void onSetMinSensorValue(uint8_t sensorID, uint16_t value);
   void onSetMaxSensorValue(uint8_t sensorID, uint16_t value);
   void onSetRCL(uint8_t chan, const char* rclCode);

#ifdef QUARKTX_TEST
   void onNewCommand(const char* cmdStr);
   void onEvaluateExpression();
   uint16_t getOutputPPM(uint8_t channel) {return ppmOutputValue_[channel];}
   Sensor *getSensor(uint8_t idx) { return sensor_[idx];}
   BatteryMeter *getBattery() {return &battMeter_;}
#endif

   // Public functions
   void setup();
   void loop();
   Model* getCurrentModel() const {return currentModel_;}
   uint8_t getModelIndex(Model *model);
   uint8_t getSensorIndex(uint8_t pinPort) const;
};

#endif
