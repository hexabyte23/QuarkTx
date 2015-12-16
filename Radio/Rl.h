#ifndef RL_H
#define RL_H

#include <inttypes.h>
#include "config.h"

struct StreamBase
{
  virtual uint16_t getValue() = 0;
};

struct InputStream : public StreamBase
{
  uint16_t *data_;
  InputStream(uint16_t *data) : data_(data) {}
  virtual uint16_t getValue() {return *data_;}
};

struct OperatorBase
{
  StreamBase *i1_, *i2_;
  
  OperatorBase(StreamBase *i1, StreamBase *i2):i1_(i1), i2_(i2) {}
  virtual StreamBase* evaluate() = 0;
};

struct OrOp : public OperatorBase
{
  OrOp(StreamBase *i1, StreamBase *i2);
  virtual StreamBase* evaluate();
};

class Rl
{
  InputStream i1_;
  OperatorBase *op_[MAX_PPM_OUTPUT_CHANNEL];
  uint16_t *inputValue_;
  uint16_t *outputValue_;
  
  public:

  Rl();

  void setup(uint16_t *inputValue, uint16_t *outputValue) {inputValue_=inputValue;outputValue_=outputValue;}
  void parse(const char *def);
  void evaluate();
};

#endif
