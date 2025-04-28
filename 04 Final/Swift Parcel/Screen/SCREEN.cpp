#include "SCREEN.h"
#include "Arduino.h"

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define MINPRESSURE 100
#define MAXPRESSURE 800

// Touchscreen calibration
#define TS_LEFT 112
#define TS_RT   944
#define TS_TOP  94
#define TS_BOT  932

// Button dimensions
int button1x = 40, button1y = 40, buttonWidth1 = 120, buttonHeight1 = 60;
int button2x = 40, button2y = 140, buttonWidth2 = 120, buttonHeight2 = 60;
int button3x = 310, button3y = 170, buttonWidth3 = 100, buttonHeight3 = 40;

Screen::Screen()
  : ts(TouchScreen(9, A2, A3, 8, 300)),
    buttonState1(false), buttonState2(false), buttonState3(true) {}

void Screen::init() {
  uint16_t id = tft.readID();
  tft.begin(id);
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  tft.setCursor(40, 20);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Solenoid 1");

  tft.setCursor(40, 120);
  tft.setTextSize(2);
  tft.println("Solenoid 2");

  tft.setCursor(40, 280);
  tft.setTextColor(WHITE);
  tft.setTextSize(1.5);
  tft.println("Parcel Dropper 2024");

  drawButtons();
}

bool Screen::Touch_getXY() {
  TSPoint p = ts.getPoint();
  pinMode(A2, OUTPUT); // YP
  pinMode(A3, OUTPUT); // XM
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (pressed) {
    pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());  
    pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height()); 
  }
  return pressed;
}

void Screen::handleTouch() {
  static bool lastTouchState = false;
  bool currentTouchState = Touch_getXY();

  if (currentTouchState && !lastTouchState) {
    if (pixel_x > button1x && pixel_x < button1x + buttonWidth1 &&
        pixel_y > button1y && pixel_y < button1y + buttonHeight1) {
      buttonState1 = !buttonState1;
    } else if (pixel_x > button2x && pixel_x < button2x + buttonWidth2 &&
               pixel_y > button2y && pixel_y < button2y + buttonHeight2) {
      buttonState2 = !buttonState2;
    } else if (pixel_x > button3x && pixel_x < button3x + buttonWidth3 &&
               pixel_y > button3y && pixel_y < button3y + buttonHeight3) {
      buttonState3 = !buttonState3;
    }
    drawButtons();
  }

  lastTouchState = currentTouchState;
}

void Screen::drawButtons() {
  // Solenoid 1
  tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, buttonState1 ? MAGENTA : CYAN);
  tft.setCursor(button1x + 10, button1y + 20);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(buttonState1 ? "OPEN" : "CLOSE");

  // Solenoid 2
  tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, buttonState2 ? MAGENTA : CYAN);
  tft.setCursor(button2x + 10, button2y + 20);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(buttonState2 ? "OPEN" : "CLOSE");

  // Barcode Button
  tft.fillRect(button3x, button3y, buttonWidth3, buttonHeight3, buttonState3 ? MAGENTA : CYAN);
  tft.setCursor(button3x + 12, button3y + 12);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.print(buttonState3 ? "SCAN" : "UNSCAN");
}

void Screen::update() {
  handleTouch();
}


void Screen::showIntroduction() {
  tft.setCursor(80,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Swift Dropper");

  tft.setCursor(90,100);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Fast, Simple, Easy");
}

bool Screen::showBinStatus(bool status) {
  // Shows tft screen full or not 
  // returns True for full or False for not full  
  if (status == 1) {
    tft.setCursor(80,140);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Bin Is Almost Full");

    Serial.println("Bin is Almost full");
  }
  else {
    tft.setCursor(80,140);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("No yet full");   

    Serial.println("Not yet full");
  }
}
