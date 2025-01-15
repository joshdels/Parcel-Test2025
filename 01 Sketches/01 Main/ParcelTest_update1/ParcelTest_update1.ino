//*************************************
//*************************************
//    Parcel Droper Hardware Test v2 update  
//    Date: 15-Jan-2025             
//    functions for adafruit 
//    https://learn.adafruit.com/adafruit-gfx-graphics-library?view=all
//*************************************
//*************************************

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>



//Colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//  PINS //
//Proximity
#define trig_pin 47
#define echo_pin 46

//Solenoid
#define solenoid_pin1 38  
#define solenoid_pin2 39  

//Variables Assignment
// For Proximity
float duration;
float distance;
const float min_distance = 3.00;  // Minimum distance in cm
const int num_samples = 5;        // Number of samples for averaging
bool stop_operation = false;      // Flag to indicate if the operation should stop

bool buttonState1 = false; // Initially set to OFF (false) for OPEN button
bool buttonState2 = false; // Initially set to OFF (false) for CLOSE button


//   Touchscreen Settings  //
// Calibrated Values
const int XP = 9, XM = A3, YP = A2, YM = 8; // 320x480 ID=0x9488
const int TS_LEFT = 112, TS_RT = 944, TS_TOP = 94, TS_BOT = 932;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); 

MCUFRIEND_kbv tft;

int pixel_x, pixel_y; // Touch coordinates

#define MINPRESSURE 100
#define MAXPRESSURE 800

// Button positions and sizes
//Button 1 - for solenoid 1
int button1x = 40, button1y = 40, buttonWidth1 = 120, buttonHeight1 = 60; 
//Button 2 - for solenoid 2 
int button2x = 40, button2y = 140, buttonWidth2 = 120, buttonHeight2 = 60;

// Touch detection function
bool Touch_getXY()
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      // restore shared pins
    pinMode(XM, OUTPUT);      // because TFT control pins
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  // Map the touch coordinates to screen width
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());  // Map the touch coordinates to screen height
        Serial.print("Touch detected: X = ");
        Serial.print(pixel_x);
        Serial.print(", Y = ");
        Serial.println(pixel_y);
    }
    return pressed;
}


void button3() {

  tft.fillRect(310, 170, 100 , 40, CYAN); //RED OFF - YELLOW ON- 

  tft.setCursor(325,180);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.println("SCAN");

  tft.setCursor(300,260);
  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.print("124-abc");
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
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Proximity Level");

  tft.fillRect(260, 40, 200 , 50, BLACK); 

  tft.setCursor(300,60);
  tft.setTextSize(3);
  tft.print(distance);
  tft.println(" cm");

  // Serial.println(distance); // for debugging

}

void barcode_panel(){
  tft.setCursor(280,140);
  tft.setTextSize(2);
  tft.println("Barcode Device");




}

void drawButtons() {
  // Button 1
  if (buttonState1) {
      tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, RED);  
      tft.setCursor(button1x + 22, button1y +20); 
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.print("OPEN");

      //Activate the Solenoid to open
      digitalWrite(solenoid_pin1, HIGH);

  } else {
      tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, CYAN); 
      tft.setCursor(button1x + 20, button1y + 20); 
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.print("CLOSE");

      // Deactivate solenoid to close
      digitalWrite(solenoid_pin1, LOW);  
  }

  // Draw Button 2
  if (buttonState2) {
      tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, RED); 
      tft.setCursor(button2x + 20, button2y + 20);  
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.print("OPEN");

      digitalWrite(solenoid_pin2, HIGH); 

  } else {
      tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, CYAN);  
      tft.setCursor(button2x + 20, button2y + 20);  
      tft.setTextColor(BLACK);
      tft.setTextSize(3);
      tft.print("CLOSE");

      digitalWrite(solenoid_pin2, LOW);  // Deactivate solenoid 2 for CLOSE
  }
}

void setup() {
  //PINMODES
  
  //Proximity
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  //Solenoid
  pinMode(solenoid_pin1, OUTPUT);
  pinMode(solenoid_pin2, OUTPUT);

  Serial.begin(9600);
  uint16_t id = tft.readID();
  tft.begin(id);
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  tft.setCursor(80,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Parcel Dropper 2024");
  delay(1000);
  
  tft.fillScreen(BLACK);

  tft.drawFastVLine(240, 0, 480, WHITE);
  tft.drawFastHLine(240, 120, 300, WHITE);

  tft.setCursor(40,20);
  tft.setTextSize(2);
  tft.println("Solenoid 1");

  tft.setCursor(40,120);
  tft.setTextSize(2);
  tft.println("Solenoid 2");

  tft.setCursor(40,280);
  tft.setTextColor(WHITE);
  tft.setTextSize(1.5);
  tft.println("Parcel Dropper 2024");

  

  show_proximity();
  barcode_panel();
  
  button3();

  drawButtons();

 
}

void loop() {
  //Proximity Checker
  distance = get_distance();
  show_proximity();

  delay(1000);


  bool down = Touch_getXY();  // Check if there is a touch on the screen

    //Button 1 conditions
    if (down && pixel_x > button1x && pixel_x < button1x + buttonWidth1 && pixel_y > button1y && pixel_y < button1y + buttonHeight1) {
        buttonState1 = !buttonState1; 
        drawButtons(); 
        delay(100);  
    }
    //Button 2 conditions
    if (down && pixel_x > button2x && pixel_x < button2x + buttonWidth2 && pixel_y > button2y && pixel_y < button2y + buttonHeight2) {
        buttonState2 = !buttonState2; 
        drawButtons();  
        delay(100);  
  }

}
