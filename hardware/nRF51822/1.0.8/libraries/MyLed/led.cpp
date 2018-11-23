#include "led.h"

LEDClass::LEDClass(byte p, bool onval)
	:cpin(p), onval(onval)
{
	pinMode(cpin, OUTPUT);
}

LEDClass::~LEDClass(void)
{
	off();
	pinMode(cpin, INPUT);
}
