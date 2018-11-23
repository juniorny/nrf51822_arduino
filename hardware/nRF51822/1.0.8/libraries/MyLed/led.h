#ifndef __LIB_LED_h__
#define __LIB_LED_h__

#include <Arduino.h>

/* class __ConstPin {
public:
    constexpr __ConstPin(const int val): val(val) {};
    constexpr operator int() const {
      return val;
    }
    const int val;
};

#define LED_FADEAMOUNT 5

typedef struct {
	int brightness;    // how bright the LED is
    int fadeAmount = LED_FADEAMOUNT;    // how many points to fade the LED by
	int fadeTime  = (1000/(255/LED_FADEAMOUNT));
} LED_TypeDef; */

class LEDClass {
public:
	LEDClass(byte p, bool onval=false);
	
	~LEDClass();
	
	void on();
	void off();
			
/* 	inline void Init(void)
	{
		pinMode(cpin, OUTPUT);
	}
	
	inline void on(void) __attribute__((always_inline)){
		digitalWrite(onval?1:0);
    }
	
	inline void off(void) __attribute__((always_inline)){
		digitalWrite(onval?0:1);
    } */
	
private:
	byte cpin;
	bool onval;
};

#endif
