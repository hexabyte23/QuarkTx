#ifndef RL_H
#define RL_H

#include <inttypes.h>
#include "config.h"

struct StreamBase
{
  uint16_t data_;
  
  StreamBase() : data_(0) {}
  
  void setValue(uint16_t data) {data_ = data;}
  virtual uint16_t getValue() {return data_;}
};

struct InputStream : public StreamBase
{
  uint16_t *dataRef_;

  InputStream() : dataRef_(NULL) {}
  
  void setup(uint16_t *dataRef) {dataRef_ = dataRef;}
  virtual uint16_t getValue() {return *dataRef_;}
};

struct OperatorBase
{
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

//
//  o0=i0
//  o1=i1|i2
//  o2=mix(i1, i2, 0.5)|i3[1;0]

class Rl
{
  InputStream i1_, i2_, i3_, i4_, i5_, i6_;
  EqualOp e1_, e2_, e3_, e4_, e5_, e6_;
  OperatorBase *op_[MAX_PPM_OUTPUT_CHANNEL] = {&e1_, &e2_, &e3_, &e4_, &e5_, &e6_};
  uint16_t *inputValue_;
  uint16_t *outputValue_;
  
  public:

  Rl();

  void setup(uint16_t *inputValue, uint16_t *outputValue);
  void parse(const char *def);
  void idle();
};

#endif
