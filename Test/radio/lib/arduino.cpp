#include "Arduino.h"

int TCCR1A;
int TCCR1B;
int OCR1A;
int OCIE1A;
int WGM12;
int CS11;
int TIMSK1;
int TCNT1;

unsigned int __heap_start;
void *__brkval;
struct __freelist *__flp;

#include <math.h>   // for isnan() and isinf()
#include <stdio.h>  // for sprintf()
#include <iostream>

// only for GCC 4.9+
#if defined(__GNUC__) && \
   (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 9))
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

// Visual Studo 2012 didn't have isnan, nor isinf
#if defined(_MSC_VER) && _MSC_VER <= 1700
#include <float.h>
#define isnan(x) _isnan(x)
#define isinf(x) (!_finite(x))
#endif

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
      n += 1;//write(*s++);
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
   return printf("\n"); //write('\r') + write('\n');
}


long map(long x, long in_min, long in_max, long out_min, long out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
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

int analogRead(uint8_t) {return 0;}
int digitalRead(uint8_t) {return 0;}
void digitalWrite(uint8_t, uint8_t) {}
void pinMode(uint8_t, uint8_t) {}

long micros() {return 0;}
long millis() {return 0;}
void cli() {}
void sei() {}
