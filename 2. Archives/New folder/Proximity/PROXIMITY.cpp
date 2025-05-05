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
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Measure the time it takes for the pulse to return to the echoPin
  long duration = pulseIn(echo, HIGH);
  // Calculate the distance in centimeters
  long distance = duration * 0.0344 / 2;


  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  delay(2000);
  return distance;
}

bool PROXIMITY::check_ceiling_limit(long distance){
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
