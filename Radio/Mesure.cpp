#include "Mesure.h"
#include "MemoryFree.h"

Mesure::Mesure() 
{
  reset();
}

void Mesure::displayStat(int maxLoop)
{
  if(loop_++ > maxLoop)
  {
    loop_ = 0;
    Serial.print(min_);
    Serial.print(" ");
    Serial.print(avg_);
    Serial.print(" ");
    Serial.print(max_);
    Serial.print(" ");
    Serial.println(freeMemory());
  }
}

void Mesure::reset()
{
  sum_ = avg_ = max_ = count_ = loop_ = 0L;
  min_ = -1;
}

