 #include "PROXIMITY.h"
 #include "SCREEN.h"

//Use 5v for sensor
const int trigPin = 25;
const int echoPin = 24;
int ceiling_limit= 50;


PROXIMITY proximity(trigPin, echoPin, ceiling_limit);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  long distance = proximity.get_distance();
  bool result = proximity.check_ceiling_limit(distance);
  Serial.println(result);
}