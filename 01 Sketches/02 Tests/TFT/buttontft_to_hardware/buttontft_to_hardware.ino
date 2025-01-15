#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define solenoid_pin1 38
#define solenoid_pin2 39

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const int XP = 9, XM = A3, YP = A2, YM = 8; // 320x480 ID=0x9488
const int TS_LEFT = 112, TS_RT = 944, TS_TOP = 94, TS_BOT = 932;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);  // Touch screen initialization

MCUFRIEND_kbv tft;  // TFT display object

// Colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

int pixel_x, pixel_y; // Touch coordinates

// Variables to track the button state
bool buttonState = false; // Initially set to OFF (false)

int buttonX = 40, buttonY = 20, buttonWidth = 160, buttonHeight = 60;  // Button position and size

// Touch detection function
bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);      // restore shared pins
    pinMode(XM, OUTPUT);      // because TFT control pins
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());  // Map the touch coordinates to screen width
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());  // Map the touch coordinates to screen height
    }
    return pressed;
}

void setup(void)
{
    pinMode(solenoid_pin1, OUTPUT);
    pinMode(solenoid_pin2, OUTPUT);

    Serial.begin(9600);
    uint16_t ID = tft.readID();

    tft.begin(ID);

    tft.setRotation(0);  // Portrait mode
    tft.fillScreen(BLACK);  // Fill the screen with black initially

    
    // Draw the initial button (OFF state)
    drawButton();
}

// Function to draw the button
void drawButton() {
    if (buttonState) {
        // Draw "ON" state
        tft.fillRect(buttonX, buttonY, buttonWidth, buttonHeight, GREEN);  // Button in green for ON state
        tft.setCursor(buttonX + 20, buttonY + 10);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("OPEN");

        digitalWrite(solenoid_pin1, HIGH);
        digitalWrite(solenoid_pin2, HIGH);

    } else {
        // Draw "OFF" state
        tft.fillRect(buttonX, buttonY, buttonWidth, buttonHeight, RED);  // Button in red for OFF state
        tft.setCursor(buttonX + 20, buttonY + 10);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("CLOSE");

        digitalWrite(solenoid_pin1, LOW);
        digitalWrite(solenoid_pin2, LOW);
    }
}

// Loop function to check touch events and toggle button
void loop(void)
{
    // tft.setCursor(0,0);
    // tft.print(p.z);

    bool down = Touch_getXY();  // Check if there is a touch on the screen

    // Check if the touch is within the bounds of the button
    if (down && pixel_x > buttonX && pixel_x < buttonX + buttonWidth && pixel_y > buttonY && pixel_y < buttonY + buttonHeight) {
        buttonState = !buttonState;  // Toggle the button state (ON/OFF)
        drawButton();  // Redraw the button with the updated state
        delay(300);  // Simple debounce delay
    }
}
