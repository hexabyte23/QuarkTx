#include "Mesure.h"


Mesure::Mesure() 
{
  reset();
}

void Mesure::displayAvg(int maxLoop)
{
  if(loop_++ > maxLoop)
  {
    loop_ = 0;
    Serial.print(min_);
    Serial.print(" ");
    Serial.print(avg_);
    Serial.print(" ");
    Serial.println(max_);
  }
}

void Mesure::reset()
{
  sum_ = avg_ = max_ = count_ = loop_ = 0L;
  min_ = -1;
}

