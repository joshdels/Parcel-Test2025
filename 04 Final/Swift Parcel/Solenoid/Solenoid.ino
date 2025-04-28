#include "Solenoid.h"

// #define solenoid_pin1 38  
// #define solenoid_pin2 39  

SOLENOID solenoid_1(28);
// SOLENOID solenoid_2(29);
// SOLENOID solenoid_3(50);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("Oy run na");
  solenoid_1.on();
  delay(5000);
  solenoid_1.off();
  Serial.println("Wala na ni run");
  delay(5000);

}
