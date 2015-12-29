/*
Mesure.cpp - QuarkTx
Copyright (c) 2015 Thierry & Betrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "SerialLink.h"
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
    STDOUT << min_ << " " << avg_ << " " << max_ << " " << freeMemory() << endl;
  }
}

void Mesure::reset()
{
  sum_ = avg_ = max_ = count_ = loop_ = 0L;
  min_ = -1;
}
