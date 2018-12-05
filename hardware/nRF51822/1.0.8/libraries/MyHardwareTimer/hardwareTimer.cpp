#include "hardwareTimer.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "math.h"


uint16_t PERIOD            = 20000 / TIMER_PRECISION;  //20ms

#ifdef NRF_TIMER0
HardwareTimer* interruptTimer0;
#endif
#ifdef NRF_TIMER1
HardwareTimer* interruptTimer1;
#endif
#ifdef NRF_TIMER2
HardwareTimer* interruptTimer2;
#endif

static void handleInterrupt(HardwareTimer *timer);

HardwareTimer::HardwareTimer(NRF_TIMER_Type *timer)
	:_timer(timer)
{

}

void HardwareTimer::pause(void)
{
	_timer->TASKS_STOP = 1;
	_timer->POWER	   = 0;
}

void HardwareTimer::resume(void)
{
	bool hasInterrupt = false;
    if (callbacks[0] != NULL) {
       hasInterrupt = true;
    }
    
#ifdef NRF_TIMER0
	if (_timer == NRF_TIMER0)
	{
		interruptTimer0 = this;
		if (hasInterrupt)
		{
			NVIC_SetPriority(TIMER0_IRQn, 1);
			NVIC_EnableIRQ(TIMER0_IRQn);
		}
	}
#endif

#ifdef NRF_TIMER1
		if (_timer == NRF_TIMER1)
		{
			interruptTimer1 = this;
			if (hasInterrupt)
			{
				NVIC_SetPriority(TIMER1_IRQn, 1);
				NVIC_EnableIRQ(TIMER1_IRQn);
			}
		}
#endif

#ifdef NRF_TIMER2
		if (_timer == NRF_TIMER2)
		{
			interruptTimer2 = this;
			if (hasInterrupt)
			{
				NVIC_SetPriority(TIMER2_IRQn, 1);
				NVIC_EnableIRQ(TIMER2_IRQn);
			}
		}
#endif
	_timer->TASKS_STOP = 1;

	_timer->POWER	  = 1;
	_timer->MODE 	  = TIMER_MODE_MODE_Timer;
	_timer->BITMODE	  = TIMER_BITMODE_BITMODE_16Bit << TIMER_BITMODE_BITMODE_Pos;
	_timer->PRESCALER = TIMER_PRESCALER;
	_timer->CC[3]	  = PERIOD;				// д╛хо20 ms

	_timer->EVENTS_COMPARE[3] = 0;

    _timer->INTENSET    = TIMER_INTENSET_COMPARE3_Msk;
    _timer->SHORTS      = TIMER_SHORTS_COMPARE3_CLEAR_Msk | TIMER_SHORTS_COMPARE3_STOP_Msk;
    _timer->TASKS_START = 1;

}

void HardwareTimer::shutdown(void)
{
	_timer->TASKS_SHUTDOWN = 1;
	_timer->POWER	  	   = 0;
}

uint32_t HardwareTimer::getPrescaleFactor(void)
{
	return pow(2, _timer->PRESCALER);
}

void HardwareTimer::setPrescaleFactor(uint32_t prescaler)
{
	// From datasheet: The PRESCALER register and the BITMODE register must only be updated when the timer is stopped
	_timer->TASKS_STOP = 1;		
	_timer->PRESCALER = prescaler;
	_timer->TASKS_START = 1;
}

uint32_t HardwareTimer::getOverflow(void)
{
	return _timer->CC[3];
}

void HardwareTimer::setOverflow(uint32_t overflow)
{
	_timer->CC[3] = overflow;
}


uint32_t HardwareTimer::setPeriod(uint32_t microseconds)
{
	if (!microseconds) {
        this->setPrescaleFactor(1);
        this->setOverflow(1);
        return this->getOverflow();
    }
    
	int prescaler;

    uint32_t period_cyc = microseconds << 4; 

    frexp((double)(period_cyc / MAX_RELOAD + 1), &prescaler);    

    uint32_t overflow = (uint32_t)(period_cyc / pow(2, prescaler));

    this->setPrescaleFactor(prescaler);
    this->setOverflow(overflow);
    return overflow;
}

static void handleInterrupt(HardwareTimer *timer) 
{   
	timer->_timer->EVENTS_COMPARE[3] = 0;

	if (timer->callbacks[0] != NULL) 
    	timer->callbacks[0]();

    timer->_timer->TASKS_START = 1;
}

#ifdef NRF_TIMER0
    HardwareTimer Timer0(NRF_TIMER0);
    
    extern "C" void TIMER0_IRQHandler(void)
	{	
		if (interruptTimer0 != NULL)
	    	handleInterrupt(interruptTimer0);
	}
#endif

#ifdef NRF_TIMER1
    HardwareTimer Timer1(NRF_TIMER1);

    extern "C" void TIMER1_IRQHandler(void)
	{	
		if (interruptTimer1 != NULL)
	    	handleInterrupt(interruptTimer1);
	}
#endif

#ifdef NRF_TIMER2
    HardwareTimer Timer2(NRF_TIMER2);

    extern "C" void TIMER2_IRQHandler(void)
	{	
		if (interruptTimer2 != NULL)
	    	handleInterrupt(interruptTimer2);
	}
#endif



