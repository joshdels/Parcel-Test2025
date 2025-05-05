#ifndef SCREEN_H
#define SCREEN_H

#ifdef ARDUINO
  #include <Arduino.h>
#endif

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>


class Screen {
  public:
    Screen();
    void init();
    void update();
    void showBinStatus(bool result);
    void showIntroduction();

  private:
    MCUFRIEND_kbv tft;
    TouchScreen ts;
    int pixel_x, pixel_y;
    bool buttonState1;
    bool buttonState2;
    bool buttonState3;

    bool Touch_getXY();
    // void drawButtons();


};

#endif
