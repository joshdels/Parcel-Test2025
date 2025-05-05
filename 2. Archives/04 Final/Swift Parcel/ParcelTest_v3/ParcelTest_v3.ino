//*********************************************************************************//
//*********************************************************************************//
//    Parcel Droper Hardware Test v3 update  
//    Date: 16-Jan-2025             
//    functions for adafruit 
//    https://learn.adafruit.com/adafruit-gfx-graphics-library?view=all
//    This composed of TFT GUI, 2 solenoid, Proximity, and Barcode Scanner
//    ATMEGA 2560 with EspWIFI, dip swith 1234 on
//**********************************************************************************//
//**********************************************************************************//

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

//Barcode Scanner
#define barcode_pin 31

//Variables Assignment
// For Proximity
float duration;
float distance;
const float min_distance = 5.00;  // Minimum distance in cm before warning red at least 2cm
const int num_samples = 5;        // Number of samples for averaging, to avoid random read
bool stop_operation = false;      

String lastBarcode = ""; // stores last scanned barcodes

bool buttonState1 = false; // Solenoid 1
bool buttonState2 = false; // Solenoid 2
bool buttonState3 = true; // Barcode Scanner

//   Touchscreen Settings  //
// Calibrated Values
const int XP = 9, XM = A3, YP = A2, YM = 8; // 320x480 ID=0x9488
const int TS_LEFT = 112, TS_RT = 944, TS_TOP = 94, TS_BOT = 932;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); 

MCUFRIEND_kbv tft;

int pixel_x, pixel_y; // Touch coordinates

#define MINPRESSURE 100
#define MAXPRESSURE 800

// Button positions and sizes //

//Button 1 - for solenoid 1
int button1x = 40, button1y = 40, buttonWidth1 = 120, buttonHeight1 = 60; 
//Button 2 - for solenoid 2 
int button2x = 40, button2y = 140, buttonWidth2 = 120, buttonHeight2 = 60;
// Button 3 - for Barcode Scanner
int button3x = 310, button3y = 170, buttonWidth3 = 100, buttonHeight3 = 40;

// Touch detection function
bool Touch_getXY()
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      // restore shared pins
    pinMode(XM, OUTPUT);      // because TFT control pins
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

    if (pressed) {
        //needs to calibrate pixel_x and pixel_y depending on you tft.setRotation()
        pixel_x = map(p.y, TS_TOP, TS_BOT, tft.width(), 0);  // Map the touch coordinates to screen width
        pixel_y = map(p.x, TS_LEFT, TS_RT, 0, tft.height());  // Map the touch coordinates to screen height
        // for debugging touch if not responding
        // Serial.print("Touch detected: X = ");
        // Serial.print(pixel_x);
        // Serial.print(", Y = ");
        // Serial.println(pixel_y);
    }
    return pressed;
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
    if (single_distance > 0 && single_distance < 400) {
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
  if (distance >= min_distance) {
    tft.setCursor(300,60);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.print(distance);
    tft.println(" cm");
    }
  else {
    tft.setCursor(300,60);
    tft.setTextSize(3);
    tft.setTextColor(CYAN);
    tft.print(distance);
    tft.println(" cm");
    }

  // Serial.println(distance); // for debugging

}

void barcode_panel(){
  tft.setCursor(280,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Barcode Device");
}

void drawButtons() {
    // Solenoid 1 button
    tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, buttonState1 ? MAGENTA : CYAN);
    tft.setCursor(button1x + 10, button1y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState1 ? "OPEN" : "CLOSE");

    // Solenoid 2 button
    tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, buttonState2 ? MAGENTA : CYAN);
    tft.setCursor(button2x + 10, button2y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState2 ? "OPEN" : "CLOSE");

    // Barcode SCAN button
    tft.fillRect(button3x, button3y, buttonWidth3, buttonHeight3, buttonState3 ? MAGENTA : CYAN);
    tft.setCursor(button3x + 12, button3y + 12);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState3 ? "SCAN" : "UNSCAN");

    // Update solenoid and barcode states
    digitalWrite(solenoid_pin1, buttonState1);
    digitalWrite(solenoid_pin2, buttonState2);
    digitalWrite(barcode_pin, buttonState3);
}

//Barcode Configurations
void scanBarcodes() {
    if (!buttonState3) return; // Only scan if the SCAN button is ON

    if (Serial2.available()) {
        String barcode = "";
        while (Serial2.available()) {
            char c = Serial2.read();
            if (c == '\n') break; // Stop reading at newline
            barcode += c;
        }

        if (barcode.length() > 0 && barcode != lastBarcode) {
            lastBarcode = barcode;
            // for debugging
            // Serial.print("Scanned Barcode: ");
            // Serial.println(barcode);  

            scanned_barcode(barcode);
        }
    }
}

void scanned_barcode(String barcode) {
    tft.fillRect(270, 260, 200, 50, BLACK); 

    tft.setCursor(280, 260);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print(lastBarcode); 

}

void setup() {
  //PINMODES
  
  //Proximity
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  //Solenoid
  pinMode(solenoid_pin1, OUTPUT);
  pinMode(solenoid_pin2, OUTPUT);

  //Barcode
  pinMode(barcode_pin, OUTPUT);

  Serial.begin(9600);
  Serial2.begin(9600);

  uint16_t id = tft.readID();
  tft.begin(id);
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  tft.setCursor(80,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Parcel Dropper 2024");
  delay(3000);
  
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
  
  drawButtons();
 
}

void loop() {
  
  //Proximity Checker

  static unsigned long lastProximityUpdate = 0;
  if (millis() - lastProximityUpdate >= 1000) { // Update proximity every second
    lastProximityUpdate = millis();
    distance = get_distance();
    show_proximity();

  }

   static bool lastTouchState = false; // Track last touch state
   bool currentTouchState = Touch_getXY();
  // Buttons toggles states 
    if (currentTouchState && !lastTouchState) {
        if (pixel_x > button1x && pixel_x < button1x + buttonWidth1 &&
            pixel_y > button1y && pixel_y < button1y + buttonHeight1) {
            buttonState1 = !buttonState1; // Toggle Solenoid 1 state
        } else if (pixel_x > button2x && pixel_x < button2x + buttonWidth2 &&
                   pixel_y > button2y && pixel_y < button2y + buttonHeight2) {
            buttonState2 = !buttonState2; // Toggle Solenoid 2 state
        } else if (pixel_x > button3x && pixel_x < button3x + buttonWidth3 &&
                   pixel_y > button3y && pixel_y < button3y + buttonHeight3) {
            buttonState3 = !buttonState3; // Toggle SCAN state
        }

        drawButtons(); // Update button states
    }

    lastTouchState = currentTouchState;
    scanBarcodes(); // Continuously check for barco 
  }


