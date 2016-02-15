#include "Arduino.h"
#include <math.h>   // for isnan() and isinf()
#include <stdio.h>  // for sprintf()
#include <iostream>

int TCCR1A = 0x00;
int TCCR1B = 0x00;
int OCR1A = 0x00;
int OCIE1A = 0x00;
int WGM12 = 0x00;
int CS11 = 0x00;
int TIMSK1 = 0x00;
int TCNT1 = 0x00;

int FTFL_FCCOB0 = 0x00;
int FTFL_FCCOB1 = 0x00;
int FTFL_FCCOB5 = 0x00;
int FTFL_FCCOB6 = 0x00;
int FTFL_FCCOB7 = 0x00;

int FTFL_FSTAT = 0x00;
int FTFL_FSTAT_CCIF = 0x00;

int SIM_SCGC6 = 0x00;
int SIM_SCGC6_PIT = 0x00;
int PIT_MCR = 0x00;
int PIT_TCTRL1 = 0x00;
int PIT_LDVAL1 = 0x00;
int PIT_TFLG1 = 0x00;

unsigned int __heap_start = 0x00;
void *__brkval = 0x00;
struct __freelist *__flp = 0x00;


size_t Print::write(uint8_t c)
{
   printf("%c", (char)c);
   return 1;
}

size_t Print::print(const char s[])
{
   size_t n = 0;
   while (*s)
   {
      printf("%c", *s++);
      n += 1;
   }
   return n;
}

size_t Print::print(double value, int digits)
{
   if (isnan(value)) return print("nan");
   if (isinf(value)) return print("inf");

   char tmp[32];

   bool isBigDouble = value > 4294967040.0 || value < -4294967040.0;

   if (isBigDouble)
   {
      // Arduino's implementation prints "ovf"
      // We prefer trying to use scientific notation, since we have sprintf
      sprintf(tmp, "%g", value);
   }
   else
   {
      // Here we have the exact same output as Arduino's implementation
      sprintf(tmp, "%.*f", digits, value);
   }

   return print(tmp);
}

size_t Print::print(unsigned long value)
{
   char tmp[32];
   sprintf(tmp, "%lu", value);
   return print(tmp);
}

size_t Print::print(long value)
{
   char tmp[32];
   sprintf(tmp, "%ld", value);
   return print(tmp);
}

size_t Print::print(int value)
{
   char tmp[32];
   sprintf(tmp, "%d", value);
   return print(tmp);
}

size_t Print::print(unsigned int value)
{
   char tmp[32];
   sprintf(tmp, "%u", value);
   return print(tmp);
}

size_t Print::println()
{
   return printf("\n");
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout)
{
   char fmt[20];
   sprintf(fmt, "%%%d.%df", width, prec);
   sprintf(sout, fmt, val);
   return sout;
}

extern char* itoa( int value, char *string, int radix )
{
   return ltoa( value, string, radix ) ;
}

extern char* ltoa( long value, char *string, int radix )
{
   char tmp[33];
   char *tp = tmp;
   long i;
   unsigned long v;
   int sign;
   char *sp;

   if ( string == NULL )
   {
      return 0 ;
   }

   if (radix > 36 || radix <= 1)
   {
      return 0 ;
   }

   sign = (radix == 10 && value < 0);
   if (sign)
   {
      v = -value;
   }
   else
   {
      v = (unsigned long)value;
   }

   while (v || tp == tmp)
   {
      i = v % radix;
      v = v / radix;
      if (i < 10)
         *tp++ = i+'0';
      else
         *tp++ = i + 'a' - 10;
   }

   sp = string;

   if (sign)
      *sp++ = '-';
   while (tp > tmp)
      *sp++ = *--tp;
   *sp = 0;

   return string;
}

extern char* utoa( unsigned long value, char *string, int radix )
{
   return ultoa( value, string, radix ) ;
}

extern char* ultoa( unsigned long value, char *string, int radix )
{
   char tmp[33];
   char *tp = tmp;
   long i;
   unsigned long v = value;
   char *sp;

   if ( string == NULL )
   {
      return 0;
   }

   if (radix > 36 || radix <= 1)
   {
      return 0;
   }

   while (v || tp == tmp)
   {
      i = v % radix;
      v = v / radix;
      if (i < 10)
         *tp++ = i+'0';
      else
         *tp++ = i + 'a' - 10;
   }

   sp = string;


   while (tp > tmp)
      *sp++ = *--tp;
   *sp = 0;

   return string;
}

static int buffer__[15];
int analogRead(uint8_t pin)
{
   return buffer__[pin];
}

int digitalRead(uint8_t pin)
{
   return buffer__[pin];
}

void analogWrite(uint8_t pin, int value)
{
/*
   if(value < 0)
      value = 0;
   if(value > 1023)
      value = 1023;
*/
   buffer__[pin] = value;
}

void digitalWrite(uint8_t pin, uint8_t value)
{
   buffer__[pin] = value;
}

void pinMode(uint8_t, uint8_t)
{

}

void analogReference(uint8_t)
{

}

void analogReadRes(uint8_t)
{

}

void analogReadAveraging(uint8_t)
{

}

void delay(int)
{

}

long micros()
{
   return 0L;
}

long millis()
{
   return 0L;
}

void cli() {}
void sei() {}
