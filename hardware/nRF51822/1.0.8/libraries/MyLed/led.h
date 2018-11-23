#ifndef __LED_h__
#define __LED_h__

#include <Arduino.h>

#define  LED_DELAY(x) delay(x)

#define LED_FADEAMOUNT 5

typedef struct 
{
	int brightness;    // how bright the LED is
    int fadeAmount = LED_FADEAMOUNT;    // how many points to fade the LED by
	int fadeTime  = (1000 / (255 / LED_FADEAMOUNT));
} LED_TypeDef;

struct _FLASH
{ 
  int timeon;
  int timeoff;
  int count;
  _FLASH(int on, int off, int cnt = 1)
	: timeon(on), timeoff(off), count(cnt){}
};	

class LEDClass {
public:
	LEDClass(byte p, bool onval=false);
	
	~LEDClass(void);
	
	inline LEDClass& operator = (bool value) __attribute__((always_inline))
	{
		digitalWrite(cpin, value);
		return *this;
	}
	
	inline LEDClass& operator << (bool value) __attribute__((always_inline))
	{
		digitalWrite(cpin, value);
		return *this;
	}
	
	inline LEDClass& operator << (int value) __attribute__((always_inline))
	{
		digitalWrite(cpin, (bool)value);
		return *this;
	}
	
	inline LEDClass& operator << (const _FLASH &arg) __attribute__((always_inline)) {
		this->flash((uint16_t)arg.timeon, (uint16_t)arg.timeoff, (uint8_t)arg.count);
		return *this;
    }
	
	inline void on(void) __attribute__((always_inline))
	{
		digitalWrite(cpin, onval ? HIGH : LOW);
	}
	
	inline void on(int val) __attribute__((always_inline))
	{ 	//analogWrite
	    uint8_t res = 16;
		uint32_t max = (1 << res) - 1;
		val &= max;
		analogWrite(cpin, (onval ? val : max - val));
	}
	
	inline void off(void) __attribute__((always_inline))
	{
		digitalWrite(cpin, onval ? LOW : HIGH);
	}
	
	void flash(uint16_t timeon, uint16_t timeoff, uint8_t cnt = 1)
	{
		for(uint8_t i = cnt; i > 0; i--)
		{
			this->on();
			if(timeon)
				LED_DELAY(timeon);
			this->off();
			if(timeoff)
				LED_DELAY(timeoff);
		}
	}
	
	void fade(uint16_t time) 
	{
		if(time < sLed.fadeTime) 
			time = sLed.fadeTime;	
        for(volatile uint16_t i = 0; i < (time / sLed.fadeTime); i++)
		{
			analogWrite(cpin, sLed.brightness);

			// change the brightness for next time through the loop:
			sLed.brightness += sLed.fadeAmount;

			// reverse the direction of the fading at the ends of the fade:
			if ((sLed.brightness <= 0) || (sLed.brightness >= 250)) 
			{
				sLed.fadeAmount = -sLed.fadeAmount;
			}
			LED_DELAY(sLed.fadeTime);
		}
	}
	
	void fade()
	{ 
		fade(1000 / (255 / LED_FADEAMOUNT));
	}
			
	
private:
	byte cpin;
	bool onval;
	LED_TypeDef  sLed;
};

#endif
