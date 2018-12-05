#ifndef __HARDWARETIMER_H__
#define __HARDWARETIMER_H__

#include "nrf.h"
#include <string.h>

#define TIMER_PRECISION 4 //4us ticks
#define TIMER_PRESCALER 6 //4us ticks  =   16Mhz/(2**6)

#define MAX_RELOAD ((1 << 16) - 1) 

class HardwareTimer {
public:
	HardwareTimer(NRF_TIMER_Type *timer);
	
	void pause(void);
	void resume(void);
	void shutdown(void);
	
	uint32_t getPrescaleFactor(void);
	void setPrescaleFactor(uint32_t prescaler);
	
	uint32_t getOverflow(void);
	void setOverflow(uint32_t overflow);
	
	uint32_t setPeriod(uint32_t microseconds);

	void attachInterrupt(void (*handler)(void)) {
		callbacks[0] = handler;
	}

    void detachInterrupt() {
		callbacks[0] = NULL;
	};
	
	
	NRF_TIMER_Type *_timer;
	//Callbacks: 0 for update, 1-4 for channels
    void (*callbacks[1])(void);
};


#ifdef NRF_TIMER0
    extern HardwareTimer Timer0;
#endif

#ifdef NRF_TIMER1
    extern HardwareTimer Timer1;
#endif

#ifdef NRF_TIMER2
    extern HardwareTimer Timer2;
#endif


#endif
