#ifndef RL_H
#define RL_H

#include <inttypes.h>
#include "config.h"

struct StreamBase
{
  uint16_t data_;
  
  StreamBase() : data_(0) {}
  StreamBase(uint16_t data) : data_(data) {}
  //StreamBase(const StreamBase &i) : data_(i.data_) {}
  
  void setValue(uint16_t data) {data_ = data;}
  virtual uint16_t getValue() {return data_;}
};

struct InputStream : public StreamBase
{
  uint16_t *dataRef_;
  
  InputStream(uint16_t *data) : dataRef_(data) {}
  virtual uint16_t getValue() {return *dataRef_;}
};

struct OperatorBase
{
  virtual StreamBase *evaluate() = 0;
};

struct EqualOp : public OperatorBase
{
  StreamBase *i1_;
    
  EqualOp(StreamBase *i1) {i1_ = i1;}
  virtual StreamBase *evaluate() {return i1_;}
};

struct OrOp : public OperatorBase
{
  StreamBase *i1_, *i2_;
    
  OrOp(StreamBase *i1, StreamBase *i2):i1_(i1), i2_(i2) {}
  virtual StreamBase *evaluate();
};


//
//  o0=i0
//  o1=i1|i2
//  o2=mix(i1, i2, 0.5)|i3[1;0]

class Rl
{
  InputStream i1_, i2_, i3_, i4_;
  EqualOp e1_, e2_, e3_, e4_;
  OperatorBase *op_[MAX_PPM_OUTPUT_CHANNEL] = {&e1_, &e2_, &e3_, &e4_};
  uint16_t *inputValue_;
  uint16_t *outputValue_;
  
  public:

  Rl();

  void setup(uint16_t *inputValue, uint16_t *outputValue) {inputValue_=inputValue;outputValue_=outputValue;}
  void parse(const char *def);
  void idle();
};

#endif
