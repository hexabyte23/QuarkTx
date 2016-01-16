#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>


#define byte uint8_t
#define boolean bool
#define F(a) a
#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A7 7

#define DEC 0
#define HEX 1

extern int TCCR1A;
extern int TCCR1B;
extern int OCR1A;
extern int OCIE1A;
extern int WGM12;
extern int CS11;
extern int TIMSK1;
extern int TCNT1;


long map(long x, long in_min, long in_max, long out_min, long out_max);
char *dtostrf (double val, signed char width, unsigned char prec, char *sout);
char* itoa( int value, char *string, int radix ) ;
char* ltoa( long value, char *string, int radix ) ;
char* utoa( unsigned long value, char *string, int radix ) ;
char* ultoa( unsigned long value, char *string, int radix ) ;

int analogRead(uint8_t);
int digitalRead(uint8_t);
void digitalWrite(uint8_t, uint8_t);
void pinMode(uint8_t, uint8_t);

long micros();
long millis();
void cli();
void sei();

class Print
{
public:
    virtual ~Print() {}

    virtual size_t write(uint8_t c);

    size_t print(const char arg[]);
    size_t print(double value, int digits = 2);
    size_t print(int arg);
    size_t print(unsigned int arg);
    size_t print(long arg);
    size_t print(unsigned long arg);
    size_t println();
};

class Stream : public Print
{
public:
    Stream() {}

    void begin(int) {}
    int available() {return 0;}
    char read() {return 0;}
};

class SerialClass : public Stream
{
public:
    SerialClass() {}

    //virtual size_t write(uint8_t) {return 0;}
};

static SerialClass Serial;
