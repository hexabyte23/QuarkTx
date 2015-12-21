#ifndef RL_H
#define RL_H

#include <inttypes.h>
#include "config.h"
#include "Model.h"
#include "Sensor.h"

struct StreamBase
{
  uint16_t data_;
  
  StreamBase() : data_(0) {}
  
  void setValue(uint16_t data) {data_ = data;}
  virtual uint16_t getValue() {return data_;}
};

struct InputStream : public StreamBase
{
  Sensor *sensorRef_;

  InputStream() : sensorRef_(NULL) {}
  
  void setup(Sensor *sensorRef) {sensorRef_ = sensorRef;}
  virtual uint16_t getValue() {return sensorRef_->getValue();}
};

struct OperatorBase
{
  StreamBase ret_;
  virtual StreamBase *evaluate() = 0;
};

struct EqualOp : public OperatorBase
{
  StreamBase *i1_;
    
  void setup(StreamBase *i1) {i1_ = i1;}
  virtual StreamBase *evaluate() {return i1_;}
};

struct OrOp : public OperatorBase
{
  StreamBase *i1_, *i2_;
    
  void setup(StreamBase *i1, StreamBase *i2){i1_=i1;i2_=i2;}
  virtual StreamBase *evaluate();
};

struct LimitOp : public OperatorBase
{
  uint16_t min_, max_;
  StreamBase *i1_;
    
  void setup(StreamBase *i1) {i1_ = i1; min_=ADC_MIN_VALUE; max_=ADC_MAX_VALUE;}
  virtual StreamBase *evaluate();
};

class Rl
{
  InputStream i1_, i2_, i3_, i4_, i5_, i6_;
  EqualOp e1_, e2_, e3_, e4_, e5_, e6_;
  LimitOp l1_, l2_;
  OperatorBase *op_[MAX_PPM_OUTPUT_CHANNEL] = {&e1_, &e2_, &e3_, &e4_, &e5_, &e6_};
  Sensor **sensorRef_;
  uint16_t *outputValue_;
  Model *currentModel_;
  
  public:

  Rl();

  void setup(Sensor **sensorRef, uint16_t *outputValue, Model *currentModel);
  void parse(const char *def);
  void idle();
};

#endif
