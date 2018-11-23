/*

*/
#include <led.h>

LEDClass LED0(9, true);

void setup() 
{

}


void loop() 
{
    LED0.on();
    delay(200);
    LED0.off();
    delay(200)
}
