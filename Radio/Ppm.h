#ifndef PPM_H
#define PPM_H

#if 0

#include <inttypes.h>

#define PPM_WORK_SIZE(channels) ((channels) + (((channels) + 1) * 4))

class Ppm
{
  void updateTimings();
  
  void isr();
  
  uint16_t pulseLen_;         // Pulse length in timer ticks.
  uint16_t pauseLen_;         // End of frame length in timer ticks.
  
  uint8_t channelCount_;      // Number of active channels.
  const uint16_t* channels_;  // External buffer with channel values, in microseconds.
  
  volatile uint16_t* channelTimings_; // Timings per channel, in timer ticks.
  
  uint8_t timingCount_;     // Number of active timings.
  uint8_t timingPos_;       // Current position in timings buffer.
  uint16_t* timings_;       // Timing values in timer ticks.
  
  uint8_t mask_;            // Mask to use for pins other than 9 and 10
  volatile uint8_t* port_;  // Input port register for pins other than 9 and 10
  
  static Ppm* instance_;    // Singleton
  
public:
	
	/*! \brief Constructs a Ppm object.
	    \param channels Number of active channels, <= p_maxChannels.
	    \param input External input buffer for channel values, in microseconds.
	    \param work Work buffer, should be (p_maxChannels + 1) * 4 elements large.
	    \param maxChannels Maximum number of channels supported.*/
	Ppm(uint8_t channels, const uint16_t* input, uint8_t* work, uint8_t maxChannels);
  void setup();
	
	/*! \brief Sets up timers and interrupts.
	    \param pin Pin to use as output pin, pins 9 and 10 are preferred and give the best result.
	    \param invert Invert the signal on true.
	    \note If precise timing is of importance then you should use either pin 9 or 10,
		      these can be toggled by the timer hardware and will give the best results. */
	void start(uint8_t pin, bool invert = false, bool debug = false);
 
	void setChannelCount(uint8_t channels);
	uint8_t getChannelCount() const;
  
	void setPulseLength(uint16_t len);
	uint16_t getPulseLength() const;
	
	void setPauseLength(uint16_t len);
	uint16_t getPauseLength() const;
	
	/* Updates channel timings, will be sent at next frame.*/
	void update();
	
	static void handleInterrupt();
};

#endif

#endif

