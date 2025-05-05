#ifndef LED_H
#define LED_H

#include <Arduino.h>

class Led
{
private:
  byte pin;
public:
  Led() {}

  Led(byte pin);

  void init();
  void init(byte defaultState);

  void on();
  void off();

};



#endif