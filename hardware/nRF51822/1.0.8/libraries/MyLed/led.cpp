#include "led.h"

LEDClass::LEDClass(byte p, bool onval)
	:cpin(p), onval(onval)
{
	pinMode(cpin, OUTPUT);
}

LEDClass::~LEDClass()
{
	off();
	pinMode(cpin, INPUT);
}

void LEDClass::on()
{
	digitalWrite(cpin, onval ? HIGH : LOW);
}

void LEDClass::off()
{
	digitalWrite(cpin, onval ? LOW : HIGH);
}
