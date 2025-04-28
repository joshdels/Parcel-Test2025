// Proximity and tft test for full or not full
#include "PROXIMITY.h"
#include "SCREEN.h"

const int trigPin = 25;
const int echoPin = 24;
int ceiling_limit = 50;

PROXIMITY proximity(trigPin, echoPin, ceiling_limit);
Screen screen;

void setup() {
  Serial.begin(9600);
  screen.init();
}

void loop() {
  long distance = proximity.get_distance();
  bool result = proximity.check_ceiling_limit(distance);
  
  screen.showIntroduction();
  screen.showBinStatus(result);
  Serial.println(result);
}
