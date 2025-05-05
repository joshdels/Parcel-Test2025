#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define solenoid_pin1 38  
#define solenoid_pin2 39  

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Touchscreen settings
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

// Button states (separate for ON/OFF)
bool buttonState1 = false; // Initially set to OFF (false) for OPEN button
bool buttonState2 = false; // Initially set to OFF (false) for CLOSE button

// Button positions and sizes
int button1x = 0, button1y = 0, buttonWidth1 = 140, buttonHeight1 = 60;  // Button position and size for OPEN
int button2x = 200, button2y = 150, buttonWidth2 = 140, buttonHeight2 = 60; // Button position and size for CLOSE

void button2() {

  tft.fillRect(40, 140, 120 , 60, CYAN); //RED OFF - YELLOW ON- 

  tft.setCursor(70,155);
  tft.setTextSize(4);
  tft.println("OFF");

  tft.setCursor(40,120);
  tft.setTextSize(2);
  tft.println("Solenoid 2");
}

void button1() {

  tft.fillRect(40, 40, 120 , 60, CYAN);
  tft.setCursor(70,60);
  tft.setTextSize(4);
  tft.println("OFF");

  tft.setCursor(40,20);
  tft.setTextSize(2);
  tft.println("Solenoid 1");


}


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

    // Draw the initial buttons (OFF state)
    drawButtons();
}

// Function to draw both buttons
void drawButtons() {
    // Draw the "OPEN" button
    if (buttonState1) {
        tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, GREEN);  // Button in green for OPEN state
        tft.setCursor(button1x + 30, button1y + 20);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("OPEN");

        digitalWrite(solenoid_pin1, HIGH); // Activate solenoid 1 for OPEN

    } else {
        tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, RED);  // Button in red for OPEN state
        tft.setCursor(button1x + 30, button1y + 20);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("OPEN");

        digitalWrite(solenoid_pin1, LOW);  // Deactivate solenoid 1 for OPEN
    }

    // Draw the "CLOSE" button
    if (buttonState2) {
        tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, GREEN);  // Button in green for CLOSE state
        tft.setCursor(button2x + 30, button2y + 20);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("CLOSE");

        digitalWrite(solenoid_pin2, HIGH); // Activate solenoid 2 for CLOSE

    } else {
        tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, RED);  // Button in red for CLOSE state
        tft.setCursor(button2x + 30, button2y + 20);  // Set the cursor to the center of the button
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.print("CLOSE");

        digitalWrite(solenoid_pin2, LOW);  // Deactivate solenoid 2 for CLOSE
    }
}

// Loop function to check touch events and toggle buttons
void loop(void)
{
    bool down = Touch_getXY();  // Check if there is a touch on the screen

    // Check if the touch is within the bounds of the "OPEN" button
    if (down && pixel_x > button1x && pixel_x < button1x + buttonWidth1 && pixel_y > button1y && pixel_y < button1y + buttonHeight1) {
        buttonState1 = !buttonState1;  // Toggle the "OPEN" button state
        drawButtons();  // Redraw the buttons with the updated states
        delay(300);  // Simple debounce delay
    }

    // Check if the touch is within the bounds of the "CLOSE" button
    if (down && pixel_x > button2x && pixel_x < button2x + buttonWidth2 && pixel_y > button2y && pixel_y < button2y + buttonHeight2) {
        buttonState2 = !buttonState2;  // Toggle the "CLOSE" button state
        drawButtons();  // Redraw the buttons with the updated states
        delay(300);  // Simple debounce delay
    }
}
