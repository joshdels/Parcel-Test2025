#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define barcode_pin 31

#define solenoid_pin1 38
#define solenoid_pin2 39

#define MINPRESSURE 200
#define MAXPRESSURE 1000

String lastBarcode = ""; // Store the last scanned barcode

// Touchscreen settings
const int XP = 9, XM = A3, YP = A2, YM = 8; // 320x480 ID=0x9488
const int TS_LEFT = 112, TS_RT = 944, TS_TOP = 94, TS_BOT = 932;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300); // Touch screen initialization
MCUFRIEND_kbv tft;                                 // TFT display object

// Colors
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

int pixel_x, pixel_y; // Touch coordinates

// Button states
bool buttonState1 = false;
bool buttonState2 = false;
bool buttonState3 = false;

// Button positions and sizes
int button1x = 40, button1y = 40, buttonWidth1 = 200, buttonHeight1 = 60;
int button2x = 40, button2y = 140, buttonWidth2 = 200, buttonHeight2 = 60;
int button3x = 40, button3y = 240, buttonWidth3 = 200, buttonHeight3 = 60;

// Touch detection function
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT); // restore shared pins
    pinMode(XM, OUTPUT); // because TFT control pins
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

// Function to draw all buttons based on their states
void drawButtons() {
    // Solenoid 1 button
    tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, buttonState1 ? GREEN : RED);
    tft.setCursor(button1x + 10, button1y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState1 ? "Solenoid 1 ON" : "Solenoid 1 OFF");

    // Solenoid 2 button
    tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, buttonState2 ? GREEN : RED);
    tft.setCursor(button2x + 10, button2y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState2 ? "Solenoid 2 ON" : "Solenoid 2 OFF");

    // Barcode SCAN button
    tft.fillRect(button3x, button3y, buttonWidth3, buttonHeight3, buttonState3 ? GREEN : RED);
    tft.setCursor(button3x + 10, button3y + 20);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print(buttonState3 ? "SCAN" : "UNSCAN");

    // Update solenoid and barcode states
    digitalWrite(solenoid_pin1, buttonState1);
    digitalWrite(solenoid_pin2, buttonState2);
    digitalWrite(barcode_pin, buttonState3);
}

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
            Serial.print("Scanned Barcode: ");
            Serial.println(barcode);

            scanned_barcode(barcode);
        }
    }
}

void scanned_barcode(String barcode) {
    tft.fillRect(80, 350, 220, 50, BLACK); 
    tft.setCursor(80, 350);

    tft.setTextColor(WHITE);
    tft.setTextSize(2);
    tft.print(barcode); 

}

void setup(void) {
    pinMode(solenoid_pin1, OUTPUT);
    pinMode(solenoid_pin2, OUTPUT);
    pinMode(barcode_pin, OUTPUT);

    Serial2.begin(9600);
    Serial.begin(9600);
    uint16_t ID = tft.readID();

    tft.begin(ID);
    tft.setRotation(0);      // Portrait mode
    tft.fillScreen(BLACK);   // Fill the screen with black initially

    drawButtons(); // Draw the initial buttons
}

void loop(void) {
    static bool lastTouchState = false; // Track last touch state
    bool currentTouchState = Touch_getXY();

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
    scanBarcodes(); // Continuously check for barcodes
}
