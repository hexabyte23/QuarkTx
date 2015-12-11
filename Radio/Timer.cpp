
#include <Arduino.h>
#include "Timer.h"

static Timer::Callback TOIE1Callback_ = NULL;
static Timer::Callback OCI1ACallback_ = NULL;
static Timer::Callback OCI1BCallback_ = NULL;

bool Timer::setup()
{
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1C = 0;
	TIMSK1 = 0;
	OCR1A = 0;
	OCR1B = 0;
	TCNT1 = 0;

  printf("Timer\t\tOK\n");
  return true;
}

void Timer::start(bool debug)
{
	TCCR1B = (TCCR1B & ~(_BV(CS12) | _BV(CS11) | _BV(CS10))) |
	         debug ? (_BV(CS12) | _BV(CS10)) :  _BV(CS11);
}

bool Timer::isRunning()
{
	return (TCCR1B & (_BV(CS12) | _BV(CS11) | _BV(CS10))) != 0;
}

void Timer::stop()
{
	TCCR1B &= ~(_BV(CS12) | _BV(CS11) | _BV(CS10));
}

void Timer::setCompareMatchIrq(bool enable, bool useOCIE1A, Callback callback)
{
	if(enable)
	{
		if(useOCIE1A)
		{
			OCI1ACallback_ = callback;
			TIMSK1 |= _BV(OCIE1A);
		}
		else
		{
			OCI1BCallback_ = callback;
			TIMSK1 |= _BV(OCIE1B);
		}
	}
	else
	{
		if(useOCIE1A)
		{
			TIMSK1 &= ~_BV(OCIE1A);
			OCI1ACallback_ = NULL;
		}
		else
		{
			TIMSK1 &= ~_BV(OCIE1B);
			OCI1BCallback_ = NULL;
		}
	}
}

void Timer::setOverflowIrq(bool enable, Callback callback)
{
	if(enable)
	{
		TOIE1Callback_ = callback;
		TIMSK1 |= _BV(TOIE1);
	}
	else
	{
		TIMSK1 &= ~_BV(TOIE1);
		TOIE1Callback_ = NULL;
	}
}

void Timer::setToggle(bool enable, bool useOC1A)
{
	if(enable)
		TCCR1A |= useOC1A ? _BV(COM1A0) : _BV(COM1B0);
	else
		TCCR1A &= useOC1A ? ~_BV(COM1A0) : ~_BV(COM1B0);
}

ISR(TIMER1_OVF_vect)
{
	if(TOIE1Callback_ != NULL)
		TOIE1Callback_();
}

ISR(TIMER1_COMPA_vect)
{
	if(OCI1ACallback_ != NULL)
		OCI1ACallback_();
}

ISR(TIMER1_COMPB_vect)
{
	if(OCI1BCallback_ != NULL)
		OCI1BCallback_();
}


