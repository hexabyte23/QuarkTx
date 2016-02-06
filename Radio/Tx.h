/*
Tx.h - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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


class Tx
{
   SerialLink serialLink_;
   Command command_;
   Model modelList_[MAX_MODEL];
   Model *currentModel_;
   BatteryMeter battery_;
   Sensor *sensor_[MAX_INPUT_CHANNEL];
   volatile uint16_t ppmOutputValue_[MAX_PPM_OUTPUT_CHANNEL];
   int inFreq_, outFreq_, inCurFreq_, outCurFreq_;

   // For mixers, dual rate, expo...
   RCLEval rcl_;

   Mesure mesure_;

   // LED
   int ledState_;
   unsigned long ledPrevMS_;

   // toggles
   enum {tTransmit, tDebug} toggleTxMode_;
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
   void dumpEEPROM();
   void dumpModel();
   void dumpSensor();
   void dumpRCL(const char* param);
   void resetModel();
   void resetSensor();
   void resetRCL();

   // Irq
   volatile boolean irqStartPulse_;
   volatile byte irqCurrentChannelNumber_;

public:

   Tx();

   bool setup();

   // Signals
   void onIsrTimerChange();
   void onChangeCurrentModel(int idx);
   void onDump(const char* param);
   void onToggleTxMode();
   void onToggleDisplayInputUpdate(int freq);
   void onToggleDisplayOutputUpdate(int freq);
   void onToggleCalibrateSensor();
   void onToggleSimulation();
   bool onLoadFromEEPROM();
   void onSaveToEEPROM();
   void onSoftwareReset(const char* param);
   void onSetTrimSensorValue(uint8_t sensorID, int value);
   void onSetMinSensorValue(uint8_t sensorID, uint16_t value);
   void onSetMaxSensorValue(uint8_t sensorID, uint16_t value);
   void onSetSimulateSensorValue(uint8_t sensorID, uint16_t value);
   void onSetRCL(uint8_t chan, const char* rclCode);

#ifdef QUARKTX_TEST
   // For testing only
   void onNewCommand(const char* cmdStr);
   void onEvaluateExpression();
   uint16_t getOutputPPM(uint8_t channel) {return ppmOutputValue_[channel];}
   Sensor *getSensor(uint8_t idx) { return sensor_[idx];}
#endif

   // Functions
   void idle();
   Model* getCurrentModel() const {return currentModel_;}
   uint8_t getModelIndex(Model *model);
   uint8_t getSensorIndex(uint8_t pinPort);
};

#endif
