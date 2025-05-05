#include "SCREEN.h"
#include "Arduino.h"

#define WHITE   0x0000
#define YELLOW    0x001F
#define CYAN     0xF800
#define MAGENTA   0x07E0
#define RED   0x07FF
#define GREEN 0xF81F
#define BLUE  0xFFE0
#define BLACK   0xFFFF

// Touchscreen calibration
#define MINPRESSURE 100
#define MAXPRESSURE 800
#define TS_LEFT 112
#define TS_RT   944
#define TS_TOP  94
#define TS_BOT  932

// Pressable Buttons configurations
int button0x = 0, button0y = 60, buttonWidth0 = 430, buttonHeight0 = 300; // black screen
int button1x = 420, button1y = 10, buttonWidth1 = 50, buttonHeight1 = 50; // courier


Screen::Screen()
  : ts(TouchScreen(9, A2, A3, 8, 300)),
  //might erase this 
    buttonState1(false), buttonState2(false), buttonState3(true) {}

void Screen::init() {
  // for initialization 
  uint16_t id = tft.readID();
  tft.begin(id);
  tft.fillScreen(WHITE);
  tft.setRotation(1);
}

bool Screen::Touch_getXY() {
  // for getting touch response, 
  //returns 1 as pressed 
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

//--------------------------------------------------------------------------------------------------------------------------------------

//HOMEPAGE
long Screen::homePage(bool isFull) {
  //Shows the introductory homepage screen page and return with a bool 1 & 2
     // 1 to direct to courier passcode pin 
     // 2 for user to scan barcodes
  tft.fillScreen(WHITE);
  tft.setCursor(100,100);
  tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.println("Swift Dropper");

  tft.setCursor(130,140);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.println("Fast, Simple, Easy");

  tft.fillRect(button1x, button1y, buttonWidth1, buttonHeight1, BLACK);
  tft.setCursor(button1x + 15, button1y + 15);
  tft.setTextSize(3.5);
  tft.setTextColor(WHITE);
  tft.print("i");

  bool showText = true;
  unsigned long previousMillis = 0;
  const long blinkInterval = 500;

  while (true) {
    unsigned long currentMillis = millis();

    // Blinking text
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      showText = !showText;
      tft.setTextColor(showText ? BLACK : WHITE);
      tft.setCursor(100,240);
      tft.setTextSize(2);
      tft.println("Press Anywhere to Start");
    }

    
    if (Touch_getXY()) {
      if (pixel_x > button1x && pixel_x < button1x + buttonWidth1 &&
          pixel_y > button1y && pixel_y < button1y + buttonHeight1) {
        Serial.println("PASSCODE PIN");

        tft.fillScreen(WHITE);
        tft.setCursor(120,100);
        tft.setTextColor(BLACK);
        tft.setTextSize(4);
        tft.println("Courier Pin");

        tft.setCursor(120,140);
        tft.setTextColor(BLACK);
        tft.setTextSize(2);
        tft.println("Preparing Passcode Pin");

        tft.setCursor(125,200);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("Enter the correct pin");

        tft.setCursor(110,230);
        tft.setTextColor(RED);
        tft.setTextSize(2);
        tft.println("to open the courier bin");

        delay(3000);
        return 1;
      }
      else if (pixel_x > button0x && pixel_x < button0x + buttonWidth0 &&
               pixel_y > button0y && pixel_y < button0y + buttonHeight0) {
        Serial.println("Barcode UI");

        // add check bin is full here
        if (isFull) {
          showBinStatus();
          break;

        } else {
          tft.fillScreen(WHITE);
          tft.setCursor(30,120);
          tft.setTextColor(BLACK);
          tft.setTextSize(4);
          tft.println("Preparing Barcodes");

          tft.setCursor(70,160);
          tft.setTextColor(BLACK);
          tft.setTextSize(2);
          tft.println("Please wait for a few seconds");
          delay(3000);
          return 2;
        }
      }
    }
  }
}

// ------------------------------------------------------------------------------------------------------------------------

//BARCODE SECTION
int button2x = 300, button2y = 50, buttonWidth2 = 140, buttonHeight2 = 80; // Scan
int button3x = 300, button3y = 180, buttonWidth3 = 140, buttonHeight3 = 80; // Cancel

bool Screen::barcodeUI() {
  // displays the barcode UI for the seller to interact with two buttons
    // "Scan Buttons" --> activate the Scanner
    // "Cancel Button" --> exits the barcode UI

  bool running = true;

  while (running) {
    // Draw UI
    tft.fillScreen(WHITE);
    tft.setCursor(30,30);
    tft.setTextColor(BLACK);
    tft.setTextSize(3);
    tft.println("Parcel Scanner");

    tft.setCursor(40,100);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Once Ready");

    tft.setCursor(40,120);
    tft.setTextColor(GREEN);
    tft.setTextSize(3);
    tft.println("Press SCAN");

    tft.setCursor(40,200);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Done or Cancel");

    tft.setCursor(40,220);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.println("Press EXIT");

    tft.fillRect(button2x, button2y, buttonWidth2, buttonHeight2, GREEN);
    tft.setCursor(button2x + 30, button2y + 30);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print("SCAN");

    tft.fillRect(button3x, button3y, buttonWidth3, buttonHeight3, RED);
    tft.setCursor(button3x + 30, button3y + 30);
    tft.setTextSize(3);
    tft.setTextColor(BLACK);
    tft.print("EXIT");

    // Wait for user input
    while (true) {
      if (Touch_getXY()) {
        delay(300);

        // SCAN Button
        if (pixel_x > button2x && pixel_x < button2x + buttonWidth2 &&
            pixel_y > button2y && pixel_y < button2y + buttonHeight2) {
          Serial.println("SCAN button pressed");
          scannerActivate();

          String scanned = barcode.activateScanner();
          Serial.println(scanned);

          if (scanned.length() > 0) {
            // once barcode scanner captures details needs to de bugggg
            // i think dito yung array ibutang then i butang sa if scanned == barcodeFetch[]
            // for loop dayon foreach?? 

            if (scanned == "6912670750298") {
              barcodeFound(scanned);
              seller.open();
              delay(1000);
              seller.close();
              scanned = "";
            } else {
              barcodeNotFound();
            }
          }
          break;  
        }

        // EXIT Button
        if (pixel_x > button3x && pixel_x < button3x + buttonWidth3 &&
            pixel_y > button3y && pixel_y < button3y + buttonHeight3) {
          Serial.println("EXIT pressed. returning to homepage");
          endedTransaction();
          running = false;  // Exit loop
          break;
        }
      }
    }
  }

  return false;
}

//--------------------------------------------------------------------------------------------------------------

// needs to remove
void Screen::showBarcodeData(String barcode="") {
  //This show text is scanned and its barcode
    delay(2000);
    tft.fillScreen(WHITE);

    tft.setCursor(160,90);
    tft.setTextColor(GREEN);
    tft.setTextSize(4);
    tft.println("Scanned Sucessfully"); 

    tft.setCursor(140,140);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println(barcode); 

    tft.setCursor(140,160);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Please insert your bin chute"); 

    delay(5000);
}
//----------------------------------------------

//SERVER CONNECTION
// optional but if not needs to remove
void Screen::wifiStatus(bool connection = 0) {
  //shows user that the parcel box is disconnected to the server
  if (connection == 1) {
    delay(5000);
    tft.fillScreen(WHITE);

    tft.setCursor(180,90);
    tft.setTextColor(RED);
    tft.setTextSize(5);
    tft.println("SOORY"); 

    tft.setCursor(100,140);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.println("Server not connected");

    tft.setCursor(120,220);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println("Please try again later");

    Serial.println("server not connected");

  }
  else {
    Serial.println("Connected to wifi");
  }
  delay(10000);
}

// --------------------------------------------------------------------------------------------------------------------------------------

//PROXIMITY SECTION
void Screen::showBinStatus() {
  // Shows Bin is full or not 
    tft.fillScreen(WHITE);
    tft.setCursor(120,100);
    tft.setTextColor(RED);
    tft.setTextSize(4);
    tft.println("Bin is full");

    tft.setCursor(120,150);
    tft.setTextColor(RED);
    tft.setTextSize(2.5);
    tft.println("Cannot Accept Parcels"); 

    tft.setCursor(130,220);
    tft.setTextColor(BLACK);
    tft.setTextSize(2.5);
    tft.println("Try again later :)"); 

    delay(3000);
}

//--------------------------------------------------------------------------------------------------------------------------

//END OF TRANSACTION SECTION

void Screen::endedTransaction() {
  //This show text that the transaction is ended

    tft.fillScreen(WHITE);
    tft.setCursor(100,100);
    tft.setTextColor(BLACK);
    tft.setTextSize(4);
    tft.println("Swift Dropper");

    tft.setCursor(140,150);
    tft.setTextColor(GREEN);
    tft.setTextSize(2.5);
    tft.println("Transaction Ended"); 

    tft.setCursor(80,220);
    tft.setTextColor(BLACK);
    tft.setTextSize(2.5);
    tft.println("We are happy to served you :)"); 

    delay(3000);
}

void Screen::barcodeFound(String barcode) {
  //This show text that the transaction is ended
    tft.fillScreen(WHITE);

    tft.setCursor(130,70);
    tft.setTextColor(GREEN);
    tft.setTextSize(5);
    tft.println("Success!");

    tft.setCursor(60,120);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Parcel is found in the system"); 

    tft.setCursor(150,150);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println(barcode); 

    tft.setCursor(110,220);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Put your Scanned Parcel"); 

    tft.setCursor(170,240);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("In the Bin"); 

    delay(3000);
}

void Screen::barcodeNotFound() {
  //Shows the parcel's barcode is not in the system
    delay(800);

    tft.fillScreen(WHITE);

    tft.setCursor(80,80);
    tft.setTextColor(RED);
    tft.setTextSize(4);
    tft.println("Not Registered");

    tft.setCursor(40,120);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Parcel is not found in the System"); 

    tft.setCursor(80,210);
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println("Did you register your Parcel?");

    tft.setCursor(40,240);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.println("Please contact the customer support"); 

    delay(3000);
}

void Screen::showCourierBin() {
    tft.fillScreen(WHITE);
    tft.setCursor(140, 120);
    tft.setTextColor(GREEN);
    tft.setTextSize(4);
    tft.print("Bin Opened");

    tft.setCursor(50, 180);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("Courier can now collect the parcel");

    delay(3000);
}

void Screen::scannerActivate() {

    tft.fillScreen(WHITE);
    tft.setCursor(140, 80);
    tft.setTextColor(RED);
    tft.setTextSize(4);
    tft.print("Scanning");

    tft.setCursor(50, 180);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("Please place your parcel's barcode");

    tft.setCursor(120, 210);
    tft.setTextColor(BLACK);
    tft.setTextSize(2);
    tft.print("At the barcode scanner");

    delay(3000);


}





