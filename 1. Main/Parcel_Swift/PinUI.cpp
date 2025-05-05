#include "PinUI.h"

#define WHITE   0x0000
#define YELLOW    0x001F
#define CYAN     0xF800
#define MAGENTA   0x07E0
#define RED   0x07FF
#define GREEN 0xF81F
#define BLUE  0xFFE0
#define BLACK   0xFFFF

#define TS_LEFT 112
#define TS_RT   944
#define TS_TOP  94
#define TS_BOT  932


PinUI::PinUI(MCUFRIEND_kbv& _tft, TouchScreen& _ts, const String& _correctPin)
  : tft(_tft), ts(_ts), currentInput(""), correctPin(_correctPin) {}

bool PinUI::getTouch() {
    //returns true if touched
    TSPoint p = ts.getPoint();
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    bool pressed = (p.z > 200 && p.z < 1000);
    if (pressed) {
        pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());
        pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
    }
    return pressed;
}

void PinUI::drawButton(const Button& b, uint16_t color) {
    tft.fillRect(b.x, b.y, 80, 60, color);
    tft.drawRect(b.x, b.y, 80, 60, BLACK);
    tft.setCursor(b.x + 10, b.y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(b.label);
}

void PinUI::drawPinDisplay() {
    tft.fillRect(150, 20, 200, 30, WHITE);
    tft.setCursor(150, 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    for (char c : currentInput) tft.print("*");
}

void PinUI::drawMessage(const String& msg, uint16_t color) {
    // writes correct or incorrect passcode check
    tft.fillRect(150, 280, 200, 20, WHITE);
    tft.setCursor(150, 280);
    tft.setTextColor(color);
    tft.setTextSize(2);
    tft.print(msg);
    delay(2000);
    tft.fillRect(150, 280, 200, 40, WHITE);
}

void PinUI::drawUI() {
    //Draw the UI of the passcode for courier to unlock the Door
    tft.fillScreen(WHITE);
    tft.setCursor(20, 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("Enter PIN:");

    int startX = 60, startY = 70, gap = 10, label = 1;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int idx = row * 3 + col;
            buttons[idx] = {startX + col * (80 + gap), startY + row * (60 + gap), String(label)};
            drawButton(buttons[idx]);
            label++;
        }
    }

    int rightX = tft.width() - 80 - 10;
    buttons[9] = {rightX, startY, "Enter"};   drawButton(buttons[9], GREEN);
    buttons[10]  = {rightX, startY + 1 * (60 + gap), "Clear"}; drawButton(buttons[10], WHITE);
    buttons[11] = {rightX, startY + 2 * (60 + gap), "Exit"}; drawButton(buttons[11], RED);

    drawPinDisplay();
}

void PinUI::handleTouch() {
    //This will handle if the passcode UI is pressed
    // return 1 if pressed and return 0 if not pressed
    if (!getTouch()) return;

    for (int i = 0; i < 12; i++) {
        Button b = buttons[i];
        if (pixel_x > b.x && pixel_x < b.x + 80 && pixel_y > b.y && pixel_y < b.y + 60) {
            if (i < 9 && currentInput.length() < 6) {  // Digits 1-9
                currentInput += b.label;
                drawPinDisplay();
                delay(200);
            } else if (i == 9) { 
                if (currentInput == correctPin) { 
                    drawMessage("Correct", GREEN);
                    verified = true;  
                } else {
                    drawMessage("Incorrect", RED);  
                    delay(1000);  
                    currentInput = "";  
                    drawPinDisplay();  
                }
                delay(800);  

            } else if (i == 10) {  
                currentInput = "";
                drawPinDisplay();
                delay(500);

            } else if (i == 11) {  
                currentInput = "";
                cancelled = true;
                tft.fillScreen(WHITE);
                tft.setCursor(160, 120);
                tft.setTextColor(RED);
                tft.setTextSize(4);
                tft.print("Exiting");

                tft.setCursor(110, 180);
                tft.setTextColor(BLACK);
                tft.setTextSize(2);
                tft.print("Returning to homepage");

                delay(2000);
            }
            break;
        }
    }
}


bool PinUI::isVerified() {
    return verified;
}

bool PinUI::isCancelled() {
    return cancelled;
}

void PinUI::setup() {
    //intialize, construct, and draw the passcode ui for courier
    verified = false;
    cancelled = false;
    currentInput = "";
    drawUI();
}


void PinUI::update() {
    handleTouch();
}



