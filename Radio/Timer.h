#ifndef TIMER_H
#define TIMER_H

#include <inttypes.h>

class Timer
{
  Timer();
    
  public:

	typedef void (*Callback)(void); 
	
	static bool setup();
	static void start(bool debug=false);  // debug=true -> speed / 256
	static void stop();
  static bool isRunning();
	
	/*! \brief Enables/Disables Compare Match Interrupt.
	    \param enable Whether to enable or disable Compare Match Interrupt.
	    \param useOCIE1A Whether to use OCIE1A or OCIE1B.
	    \param callback Function to call at interrupt.*/
	static void setCompareMatchIrq(bool enable, bool useOCIE1A, Callback callback = NULL);
		
	/*! \brief Enables/Disables Overflow Interrupt.
	    \param enable Whether to enable or disable Overflow Interrupt.
	    \param callback Function to call at interrupt.*/
	static void setOverflowIrq(bool enable, Callback callback = NULL);
	
	/*! \brief Enables/Disables Toggle pin on Compare Match A.
	    \param enable Whether to enable or disable toggle pin.
	    \param useOC1A Whether to toggle OC1A or OC1B.*/
	static void setToggle(bool enable, bool useOC1A);
};

#endif
