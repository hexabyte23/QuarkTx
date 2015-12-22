#ifndef MESURE_H
#define MESURE_H

#include <arduino.h>

class Mesure
{
  unsigned long sum_, avg_, min_, max_, count_, p1_, loop_;

  public:

  Mesure();

  void start() {p1_ = micros();}
  void stop() 
  {
    uint32_t delta = micros() - p1_;
    
    count_++;
    sum_ += delta;
    
    avg_ = sum_/count_;
    
    if(min_ > delta)
      min_ = delta;
    if(max_ < delta)
      max_ = delta;
  }

  uint32_t getAverage() {return avg_;}

  void displayAvg(int maxLoop);
  void reset();
};

#endif
