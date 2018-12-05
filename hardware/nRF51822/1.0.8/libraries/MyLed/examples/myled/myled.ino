/*
  Copyright (c) 2018 zhong 
*/
#define BLE400
#include <led.h>

void setup() 
{
    B_LED0.on();
    B_LED1.off();
    B_LED2 << HIGH;
    B_LED3.flash(500, 200, 5);

}


void loop() 
{
    B_LED4.fade();
}



