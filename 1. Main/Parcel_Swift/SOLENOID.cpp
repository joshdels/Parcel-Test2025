#include "SOLENOID.h"
#include "Arduino.h"

SOLENOID::SOLENOID(int solenoidPin){
  pin = solenoidPin;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void SOLENOID::close(){
  //Close the solenoid
  digitalWrite(pin, HIGH);
  delay(2000);
}

void SOLENOID::open(){
  //Opens the solenoid
  digitalWrite(pin, LOW);
  delay(2000);
}