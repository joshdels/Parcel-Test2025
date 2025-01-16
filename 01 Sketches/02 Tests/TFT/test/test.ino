#include <MCUFRIEND_kbv.h>
// #include <Adafruit_GFX.h>  // Include this line for predefined colors
MCUFRIEND_kbv tft;

#define RED     0xF800    // RGB565 value for Red
#define GREEN   0x07E0    // RGB565 value for Green
#define BLUE    0x001F    // RGB565 value for Blue

void setup() {
    Serial.begin(9600);
    Serial.begin(9600);
    Serial.println("Starting...");
    uint16_t ID = tft.readID();
    Serial.print("Display ID: 0x");
    Serial.println(ID, HEX);
    tft.begin(0x9488);  // Use your display's correct ID
    tft.fillScreen(RED); // Use RED from Adafruit_GFX
}

void loop() {
    // Test display by filling with different colors
    delay(1000);
    tft.fillScreen(GREEN);  // Use GREEN from Adafruit_GFX
    delay(1000);
    tft.fillScreen(BLUE);   // Use BLUE from Adafruit_GFX
    delay(1000);
    tft.fillScreen(RED);
}
