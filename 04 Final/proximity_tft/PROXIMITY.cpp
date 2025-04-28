#include "PROXIMITY.h"
#include "Arduino.h"

PROXIMITY::PROXIMITY(int trigPin, int echoPin, int ceiling_limit){
  trig = trigPin;
  echo = echoPin;
  this->ceiling_limit = ceiling_limit;

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}
// methods or function needs to update
long PROXIMITY::get_distance(){
  // Returns distance from the base in centimeters
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH);
  long distance = duration * 0.0344 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(5000);
  return distance;
}

bool PROXIMITY::check_ceiling_limit(long distance){
  // Returns true if bin is full, returns false if bin not yet full
  if (ceiling_limit > distance){
    Serial.println("Bin is almost full!");
    return true;
  }
  else{
    Serial.println("Bin is not yet full");
    return false;
  }
  ;

}
