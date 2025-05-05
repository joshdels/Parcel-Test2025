#ifndef PINUI_H
#define PINUI_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

class PinUI {
public:
    PinUI(MCUFRIEND_kbv& tft, TouchScreen& ts, const String& _correctPin);
    bool up;
    void setup();
    void update();
    bool isVerified();
    bool isCancelled();
    bool verified;
    bool cancelled;

    MCUFRIEND_kbv& tft;
    TouchScreen& ts;

private:
    struct Button {
        int x, y;
        String label;
    };

    int pixel_x, pixel_y;
    String currentInput;
    String correctPin;
    Button buttons[12];

    bool getTouch();
    void drawButton(const Button& b, uint16_t color = 0x7BEF);
    void drawPinDisplay();
    void drawMessage(const String& msg, uint16_t color = 0x07E0);
    void drawUI();
    void handleTouch();
};

#endif
