/*

NOTE Board ATMEGA SWITCH
  1. Switch GUIDE
    a. SWITCH ON 3,4 to send data, the rest are OFF
    b. SWITCH 1,2,3,4 to interact via ATMEGA <--> ESP <--> USB port
      ESP sends the fetched data, ATMEGA receives the data

  2. Screen GUIDE (TFT)
    0. Homepage - SWIFTDROPPER 
      1. SellerUI - Barcode Scanning and confirmation
      2. CourierUI - Passcode Pin (i) 

*/

#include "PROXIMITY.h"
#include "SCREEN.h"
#include "BARCODE.h"
#include "SOLENOID.h"
#include "PinUI.h"

const int trigPin = 25;
const int echoPin = 24;
const int barcodePin = 50;
const int sellerPin = 28;
const int courierPin = 29;

String scannedBarcode = "";
long user_choice;
int state = 0;
bool pinVerified = false;
bool wifiConnected = false;
bool serverConnected = false;
bool isFull = true;

// -------------------------------------------  EDITABLE SECTION  --------------------------------------------------------------------------------------

String courierPasscode = "1234"; // --> for courier pin 
int ceiling_limit = 40; // --> allowable height in centimeters
const int maxBarcodes = 500; //num of size of barcodes, lower number if board memory is full

//------------------------------------------------------------------------------------------------------------------------------------------------------

Screen screen;
String barcodeList[maxBarcodes];
PROXIMITY proximity(trigPin, echoPin, ceiling_limit);
SOLENOID courier(courierPin);
SOLENOID seller(sellerPin);
BARCODE barcode(barcodePin);
PinUI pinUI(screen.tft, screen.ts, courierPasscode);

// -------------------------------------------------------------  HANDLER  ----------------------------------------------------------------------------------

bool PinUI_passCodeUI(){
  // Handles Logic for opening the passcodePin UI
  // This will create a UI for courier 
    // if String courier pin matched to the passcode UI, the courier bin will unlock at the back
  pinUI.setup();

  while (true) {
    pinUI.update();

    if (pinUI.isVerified()) {
      // Courier PIN matched, thus opening the Courier Bin
      Serial.println("PIN Verified!");
      screen.showCourierBin();

      delay(1000);
      courier.open();
      delay(2000);
      courier.close();
      delay(2000);
      
      pinUI.verified = false;
      state = 0;
      return true;  
    }

    if (pinUI.isCancelled()) {
      // exits the passcode pin and returns back to homepage
      Serial.println("PIN Entry Cancelled.");
      delay(1000);
      return false;  
    }
      delay(50); 
    }
  state = 0;
}

void proximityHandler() {
  // handles the proximity distance
  // return 1 for full or 0 for not full
  long distance = proximity.get_distance();
  isFull = proximity.check_ceiling_limit(distance);
}

int fetchServerData() {
  // receives the data from esp then place the data to the barcodeList static array

  int barcodeCount = 0;
  bool isBarcodeSection = false;

  Serial.println("Waiting data from ESP8266...");

     static String espBuffer = "";

    while (Serial3.available()) { //Serial 3 for mega
      char c = Serial3.read();

      if (c == '\n') {
        espBuffer.trim();

        if (espBuffer == "=== WiFi Connection Start ===") {
          Serial.println("WiFi connection started...");
          wifiConnected = true;
        } else if (espBuffer == "=== WiFi Connection End ===") {
          Serial.println("WiFi connection ended.");
        } else if (espBuffer == "=== Fetching Server Data Start ===") {
          Serial.println("Server data fetch started...");
          serverConnected = true;
        } else if (espBuffer == "=== Fetching Server Data End ===") {
          Serial.println("Server data fetch ended.");
        } else if (espBuffer == "=== Barcode List Start ===") {
          Serial.println("Barcode list started...");
          isBarcodeSection = true;
          barcodeCount = 0; // reset on each new list
        } else if (espBuffer == "=== Barcode List End ===") {
          Serial.println("Barcode list ended.");
          isBarcodeSection = false;

          // Display all collected barcodes
          Serial.println("Collected Barcodes:");
          for (int i = 0; i < barcodeCount; i++) {
            Serial.println(barcodeList[i]);
          }
        } else if (espBuffer.length() > 0) {
          if (isBarcodeSection && barcodeCount < maxBarcodes) {
            barcodeList[barcodeCount++] = espBuffer;
          } else {
            // Serial.println(espBuffer);
          }
        }

        espBuffer = ""; 
      } else {
        espBuffer += c;
      }
    }
    delay(500);
}
  
//---------------------------------------------------------------------------- MAIN SET UP  ---------------------------------------------------------------------------

void setup() {
  Serial.begin(9600); 
  Serial3.begin(9600); // for UART for ESP 8266 fetched data
  screen.init();
}

void loop() {
  // States
    // 0 --> for accessing the homepage (default)
    // 1 --> for courier pincode ui
    // 2 --. for general seller ui

  Serial.println("Initializing SwiftDropper");

  int barcodeCount = fetchServerData();
  proximityHandler();

  //debugging
  Serial.print("Current state: ");
  Serial.println(state);

  if (wifiConnected) {}

  switch (state) {
    case 0: {
      Serial.println("State 0: Displaying Home Page");

      Serial.print("Box is full?");
      Serial.println(isFull);
      long user_choice = screen.homePage(isFull);


      delay(500);

      if (user_choice == 1) {
        Serial.println("Courier UI");
        state = 1;
      } else if (user_choice == 2) {
         Serial.println("Seller UI");
          if (!wifiConnected) {
            screen.wifiStatus(wifiConnected);
            break;
          }
          else if (!serverConnected) {
            screen.serverStatus(serverConnected);
            break;
          }
        state = 2;
      } else {
        Serial.println("Returning to Homepage");
      }
      break;
    }

    case 1: {
      Serial.println("State 1: Launching Courier PIN UI");
      delay(500);

      PinUI_passCodeUI();

      if (pinUI.isCancelled()) {
        Serial.println("Courier pin cancelled. Returning to homepage.");
        state = 0;
      } else {
        Serial.println("PIN entry successful. Returning to homepage.");
        state = 0;
      }
      delay(500);
      break;
    }

    case 2: {
      // Seller UI
      screen.barcodeUI(barcodeList, barcodeCount);
      delay(800);
      state = 0;
      break;
    }

    default:
      Serial.println("Back to homepage");
      state = 0;
      break;
  }
}




  