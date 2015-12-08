#include "Model.h"

#include <arduino.h>
#include "config.h"
#include "FlashMem.h"

Model::Model()
{
  for(int idx=0; idx < MAX_PPM_CHANNEL; idx++)
  {
    ppmOutput_[idx] = PPM_MID_VALUE;
  }
}

bool Model::setup(volatile int *ref)
{
  analogicSensorValueTabRef_ = ref;
}

void Model::idle()
{
  calculatePPMOutput();
}

void Model::dump()
{
  //debug("[d] dump model\n");

  Serial.print("PPM output\t");
  for(int idx =0; idx < MAX_PPM_CHANNEL; idx++)
  {
    Serial.print(ppmOutput_[idx], HEX);
    Serial.print("\t");
  }
  Serial.println();

  Serial.print("Mixer\t\t");
  for(int idx =0; idx < MAX_MIXER; idx++)
  {
    Serial.print(mixer_[idx], HEX);
    Serial.print("\t");
  }
  Serial.println();
}

void Model::calculatePPMOutput()
{
  // Get analogic imput from sensors
  for(int idx=0; idx < MAX_ADC_CHANNEL; idx++)
  {
    int v = *(analogicSensorValueTabRef_ + idx);
//    Serial.print(v, HEX);
//    Serial.print("\t");
  }
//  Serial.println();

  static int inc = 1;
  static uint32_t before = millis();
  uint32_t now = millis();

  if(now - before > 10)
  {
    before = now;
    ppmOutput_[0] += inc;
    if(ppmOutput_[0] >= PPM_MAX_VALUE) inc = -1;
    if(ppmOutput_[0] <= PPM_MIN_VALUE) inc = +1;
  }
}

