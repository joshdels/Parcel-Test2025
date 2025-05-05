//*************************************
//*************************************
//    Parcel Droper Hardware Test   
//    Date: 15-Jan-2025             
//    functions for adafruit 
//    https://learn.adafruit.com/adafruit-gfx-graphics-library?view=all
//*************************************
//*************************************

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

MCUFRIEND_kbv tft;

//Pins
#define trig_pin 47
#define echo_pin 46

float duration;
float distance;
const float min_distance = 5.00;  // Minimum distance in cm
const int num_samples = 5;        // Number of samples for averaging
bool stop_operation = false;      // Flag to indicate if the operation should stop

//Variables

//Colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


void setup() {
  //PinModes
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  Serial.begin(9600);
  uint16_t id = tft.readID();
  tft.begin(id);
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  tft.setCursor(80,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Parcel Dropper 2024");
  delay(100);
  
  tft.fillScreen(BLACK);



  tft.drawFastVLine(240, 0, 480, WHITE);
  tft.drawFastHLine(240, 120, 300, WHITE);

  tft.setCursor(40,280);
  tft.setTextColor(WHITE);
  tft.setTextSize(1.5);
  tft.println("Parcel Dropper 2024");

  button1();
  button2();
  barcode_panel();
}

void button1() {

  tft.fillRect(40, 40, 120 , 60, CYAN);
  tft.setCursor(70,60);
  tft.setTextSize(4);
  tft.println("OFF");

  tft.setCursor(40,20);
  tft.setTextSize(2);
  tft.println("Solenoid 1");


}
 
void button2() {

  tft.fillRect(40, 140, 120 , 60, CYAN); //RED OFF - YELLOW ON- 

  tft.setCursor(70,155);
  tft.setTextSize(4);
  tft.println("OFF");

  tft.setCursor(40,120);
  tft.setTextSize(2);
  tft.println("Solenoid 2");
}

void button3() {

  tft.fillRect(310, 170, 100 , 40, CYAN); //RED OFF - YELLOW ON- 

  tft.setCursor(340,180);
  tft.setTextSize(3);
  tft.println("OFF");

  tft.setCursor(40,120);
  tft.setTextSize(2);
  tft.println("Solenoid 2");
}

//Proximity Configurations
float get_distance() {
  float total_distance = 0;

  // Take multiple readings for averaging
  for (int i = 0; i < num_samples; i++) {
    // Ensure the trig_pin is LOW
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(5);

    // Send a 10-microsecond pulse to trigger the sensor
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Measure the duration of the echo in microseconds
    duration = pulseIn(echo_pin, HIGH);

    // Calculate the distance in cm
    float single_distance = (duration * 0.0343) / 2;

    // Accumulate valid readings only
    if (single_distance > min_distance && single_distance < 400) {
      total_distance += single_distance;
    } else {
      // Retry if an invalid reading is encountered (decrement i to retry)
      i--;
    }

    // Small delay between samples
    delay(10);
  }

  // Return the average distance
  return total_distance / num_samples;
}

void show_proximity() {
  tft.setCursor(280,20);
  tft.setTextSize(2);
  tft.println("Proximity Level");


  tft.setCursor(280,50);
  tft.setTextSize(3);
  tft.print(distance);
  tft.println(" cm");
}

void barcode_panel(){
  tft.setCursor(280,140);
  tft.setTextSize(2);
  tft.println("Barcode Device");

  button3();

  tft.setCursor(300,260);
  tft.setTextSize(3);
  tft.print("124-abc");


}

void loop() {
  // put your main code here, to run repeatedly:
  show_proximity();
  distance = get_distance();
  delay(1000);
}
