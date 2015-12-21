#ifndef MESURE_H
#define MESURE_H

#include <arduino.h>

class Mesure
{
  uint32_t sum_, avg_, min_, max_, count_, p1_, p2_, loop_;

  public:

  Mesure() {sum_=avg_=max_=count_=loop_=0L;min_=0xFFFFFFF0;}

  void start() {p1_ = micros();}
  void stop() 
  {
    p2_ = micros();
    count_++;
    uint32_t delta = p2_-p1_;
    sum_ += delta;
    
    avg_ = sum_/count_;
    if(min_ > delta)
      min_ = delta;
    if(max_ < delta)
      max_ = delta;
  }

  uint32_t getAverage() {return avg_;}

  void displayAvg(int maxi)
  {
    if(loop_++ > maxi)
    {
      loop_ = 0;
      Serial.print(min_);
      Serial.print(" ");
      Serial.print(avg_);
      Serial.print(" ");
      Serial.println(max_);
    }
  }
};

#endif
