#include "SCREEN.h"

Screen screen;

void setup() {
  Serial.begin(9600);
  screen.init();
}

void loop() {
  screen.update();
}
