#include "Solenoid.h"

// #define solenoid_pin1 38  
// #define solenoid_pin2 39  

SOLENOID solenoid_1(13);
SOLENOID solenoid_2(12);

void setup() {
}

void loop() {
 solenoid_1.on();
 solenoid_2.on();
 solenoid_1.off();
 solenoid_2.off();
 delay(1000);

}
