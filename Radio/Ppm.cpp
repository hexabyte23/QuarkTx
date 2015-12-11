#include <Arduino.h>
#include "Timer.h"
#include "Ppm.h"

Ppm* Ppm::instance_ = 0;

Ppm::Ppm(uint8_t channels, const uint16_t* input, uint8_t* work, uint8_t maxChannels)
:pulseLen_(500),
pauseLen_(10500),
channelCount_(channels),
channels_(input),
channelTimings_(reinterpret_cast<uint16_t*>(work)),
timingCount_((channels + 1) * 2),
timings_(const_cast<uint16_t*>(channelTimings_) + maxChannels)
{
	instance_ = this;
}

void Ppm::setup()
{
  
}

void Ppm::start(uint8_t pin, bool invert, bool debug)
{
	Timer::stop();
	
	// Fill channelTimings buffer with data from channels buffer
	update();
	
	// Fill timings buffer with data from channelTimings buffer (set up a complete PPM frame)
	updateTimings();
	
	timingPos_ = invert ? 0 : 1;
	
	pinMode(pin, OUTPUT);
	
	// First disable the output compare match A interrupt
	Timer::setCompareMatchIrq(false, true);
	
	// Configure timer1 Toggle OC1A/OC1B on Compare Match
	if (pin == 9 || pin == 10)
		Timer::setToggle(true, pin == 9);
	else
	{
		mask_ = digitalPinToBitMask(pin);
		uint8_t port = digitalPinToPort(pin);
		port_ = portInputRegister(port);
	}
	
	// set compare value
	OCR1A = TCNT1 + timings_[invert ? timingCount_ - 1 : 0];
	
	// enable timer output compare match A interrupts
	Timer::setCompareMatchIrq(true, true, Ppm::handleInterrupt);
	
	Timer::start(debug);
}

void Ppm::setChannelCount(uint8_t channels)
{
	channelCount_ = channels;
}

uint8_t Ppm::getChannelCount() const
{
	return channelCount_;
}

void Ppm::setPulseLength(uint16_t len)
{
	pulseLen_ = len << 1;
}

uint16_t Ppm::getPulseLength() const
{
	return pulseLen_ >> 1;
}

void Ppm::setPauseLength(uint16_t len)
{
	pauseLen_ = len << 1;
}

uint16_t Ppm::getPauseLength() const
{
	return pauseLen_ >> 1;
}

void Ppm::update()
{
	for(uint8_t i = 0; i < channelCount_; ++i)
		channelTimings_[i] = channels_[i] << 1;
}

void Ppm::handleInterrupt()
{
	if(instance_ != 0)
		instance_->isr();
}

void Ppm::updateTimings()
{
	uint16_t* scratch = timings_;
	
	// copy all pre-calculated timings
	for (uint8_t i = 0; i < channelCount_; ++i)
	{
		// set pulse length
		*scratch = pulseLen_;
		++scratch;
		
		// set timing
		*scratch = channelTimings_[i] - pulseLen_;
		++scratch;
	}
	
	// set final pulse length
	*scratch = pulseLen_;
	++scratch;
	
	// set pause length
	*scratch = pauseLen_ - pulseLen_;
	
	// update number of timings
	timingCount_ = (channelCount_ + 1) * 2;
}

void Ppm::isr()
{
	// set the compare register with the next value
	OCR1A += timings_[timingPos_];
	
	// toggle pin, pins 9 and 10 will toggle themselves
	if (port_ != 0)
		*port_ |= mask_;
	
	// update position
	++timingPos_;
	if (timingPos_ >= timingCount_)
	{
		timingPos_ = 0;
		
		// we're at the end of frame here, so there's plenty of time to update
		updateTimings();
	}
}

