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
#define A6 6
#define A7 7
#define A8 8
#define A9 9
#define A10 10
#define A11 11
#define A12 12
#define A13 13
#define A14 14

#define DEC 0
#define HEX 1

#define DEFAULT 0

extern int TCCR1A;
extern int TCCR1B;
extern int OCR1A;
extern int OCIE1A;
extern int WGM12;
extern int CS11;
extern int TIMSK1;
extern int TCNT1;

extern int FTFL_FCCOB0;
extern int FTFL_FCCOB1;
extern int FTFL_FCCOB5;
extern int FTFL_FCCOB6;
extern int FTFL_FCCOB7;

extern int FTFL_FSTAT;
extern int FTFL_FSTAT_CCIF;

extern int SIM_SCGC6;
extern int SIM_SCGC6_PIT;
extern int PIT_MCR;
extern int PIT_TCTRL1;
extern int PIT_LDVAL1;
extern int PIT_TFLG1;

#define NVIC_SET_PRIORITY(a, b)
#define NVIC_ENABLE_IRQ(a)

long map(long x, long in_min, long in_max, long out_min, long out_max);
char *dtostrf (double val, signed char width, unsigned char prec, char *sout);
char* itoa( int value, char *string, int radix ) ;
char* ltoa( long value, char *string, int radix ) ;
char* utoa( unsigned long value, char *string, int radix ) ;
char* ultoa( unsigned long value, char *string, int radix ) ;

int analogRead(uint8_t);
int digitalRead(uint8_t);
void digitalWrite(uint8_t, uint8_t);
void analogWrite(uint8_t, int);
void pinMode(uint8_t, uint8_t);
void analogReference(uint8_t);

void analogReadRes(uint8_t);
void analogReadAveraging(uint8_t);

void delay(int);
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
