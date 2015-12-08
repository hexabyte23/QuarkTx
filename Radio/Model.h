#ifndef MODEL_H
#define MODEL_H

#include "config.h"

class Model
{
  volatile int *analogicSensorValueTabRef_;
  int mixer_[MAX_MIXER];
  int ppmOutput_[MAX_PPM_CHANNEL];
  
  public:

  Model();

  bool setup(volatile int *ref);

  void idle();
  void dump();
  void calculatePPMOutput();
  int* getPPMOutputRef() {return ppmOutput_;}
};

#endif
