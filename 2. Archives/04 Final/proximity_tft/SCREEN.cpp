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
  tft.setRotation(1);
  tft.fillScreen(BLACK);
}

bool Screen::Touch_getXY() {
  TSPoint p = ts.getPoint();
  pinMode(A2, OUTPUT); // YP
  pinMode(A3, OUTPUT); // XM
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);

  if (pressed) {
    pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());  
    pixel_y = map(p.x, TS_LEFT, TS_RT, 0, tft.height()); 
  }
  return pressed;
}


void Screen::showIntroduction() {
  //Shows the introductory screen of the product
  tft.setCursor(80,140);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.println("Swift Dropper");

  tft.setCursor(90,100);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.println("Fast, Simple, Easy");

  delay(10);
}

void Screen::showBinStatus(bool result) {
  // Shows tft screen full or not 
  // returns True for full or False for not full  
  if (result == 1) {
    delay(5000);
    tft.fillScreen(BLACK);
    tft.setCursor(80,140);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Bin Is Almost Full");

    Serial.println("Bin is Almost full");

    
  }
  else {
    delay(5000);
    tft.fillScreen(BLACK);
    tft.setCursor(80,140);
    tft.setTextColor(WHITE);
    tft.setTextSize(3);
    tft.println("Not yet full");   

    Serial.println("Not yet full");
  }
}
