#include <hardwareTimer.h>
#define BLE400
#include <led.h>

#define Timerx Timer2

bool state;
void blinky(void)
{
    state = !state;
    if (state)
      B_LED4.on();
    else
      B_LED4.off(); 
}

void setup() {
  // nothing happens in setup
  Serial.begin(115200);
  Serial.println("Application start...");
  
  Timerx.attachInterrupt(blinky);
  Timerx.resume();
  Timerx.setPeriod(200000);
}

void loop() {
    delay(5000);

    Serial.println("----");

    Serial.print("Timer PrescaleFactor: ");
    Serial.println(Timerx.getPrescaleFactor());

    Serial.print("Timer period:");
    Serial.println(Timerx.getOverflow());

}
