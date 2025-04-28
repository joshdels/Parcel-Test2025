#include "SOLENOID.h"
#include "Arduino.h"

SOLENOID::SOLENOID(int solenoidPin){
  pin = solenoidPin;
  pinMode(pin, OUTPUT);
}

void SOLENOID::on(){
  delay(1000);
  digitalWrite(pin, HIGH);
}

void SOLENOID::off(){
  delay(1000);
  digitalWrite(pin, LOW);
}