/*
Mesure.cpp - QuarkTx
Copyright (c) 2015-2016 Thierry & Bertrand WILMOT.  All rights reserved.

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


#include <Arduino.h>
#include "MemoryFree.h"


#ifndef __MK20DX256__

extern unsigned int __heap_start;
extern void *__brkval;

/*
 * The free list structure as maintained by the
 * avr-libc memory allocation routines.
 */
struct __freelist
{
   size_t sz;
   struct __freelist *nx;
};

/* The head of the free list structure */
extern struct __freelist *__flp;

/* Calculates the size of the free list */
int freeListSize()
{
   struct __freelist* current;
   int total = 0;
   for (current = __flp; current; current = current->nx)
   {
      total += 2; /* Add two bytes for the memory block's header  */
      total += (int) current->sz;
   }

   return total;
}

#endif


int freeMemory()
{
#if defined(QT_CORE_LIB) || defined(__MK20DX256__)
   return 0;
#else
   int free_memory;
   if ((int)__brkval == 0)
   {
      free_memory = ((int)&free_memory) - ((int)&__heap_start);
   }
   else
   {
      free_memory = ((int)&free_memory) - ((int)__brkval);
      free_memory += freeListSize();
   }
   return free_memory;
#endif
}

