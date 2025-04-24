 #include "PROXIMITY.h"

//Use 5v for sensor
const int trigPin = 13;
const int echoPin = 12;
int ceiling_limit= 10;


PROXIMITY proximity(trigPin, echoPin, ceiling_limit);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  long distance = proximity.get_distance();
  proximity.check_ceiling_limit(distance);
}