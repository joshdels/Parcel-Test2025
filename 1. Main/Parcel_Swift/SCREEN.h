#ifndef SCREEN_H
#define SCREEN_H

#ifdef ARDUINO
  #include <Arduino.h>
  #include "PinUI.h"
  #include "BARCODE.h"
  #include "SOLENOID.h"
#endif

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

extern BARCODE barcode;
extern SOLENOID seller;

class Screen {
  public:
    Screen();
    void init();
    void showBinStatus();
    long homePage(bool isFull);
    void endedTransaction();
    void showBarcodeData(String barcode);
    void wifiStatus(bool wifiConnection);
    void serverStatus(bool serverConnection);
    bool barcodeUI(String barcodeList[], int count);
    void barcodeFound(String barcode);
    void barcodeNotFound();
    void showCourierBin();
    void scannerActivate();

    MCUFRIEND_kbv tft;
    TouchScreen ts;

  private:

    int pixel_x, pixel_y;
    bool buttonState1;
    bool buttonState2;
    bool buttonState3;
    bool Touch_getXY();


};

#endif
