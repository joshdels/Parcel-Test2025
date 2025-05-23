/*
For Board ESP 8266 in ATMEGA 2560 + ESP 8266 wifi dev board 
  1.) install ESP 8266 package preferences --> http://arduino.esp8266.com/stable/package_esp8266com_index.json
  2.) Switch Guide 
     a. Switches ON 5,6,7 to send the scrip to board, the rest are OFF
     b. Switches ON 5,7 to read the data in serial monitor, the rest are OFF
*/

#include "SERVER.h"

bool conditions = false;
String jsonData = "";
std::vector<String> barcodes;

// ---------------------------------------------------------- EDITABLE SECTION ---------------------------------

// const char* ssid = "PeppaPaige";
// const char* password = "Paige04102400";
// // const char* serverUrl = "http://192.168.254.105:8080/products/all";
// const char* serverUrl = "https://api.npoint.io/4ab3bc1e007126a7ba66";

const char* ssid = "PLDTHOMEFIBRiEcjt";
const char* password = "PLDTWIFInC3i3";
const char* serverUrl = "https://api.npoint.io/4ab3bc1e007126a7ba66";

//---------------------------------------------------------------------------------------------------------------

SERVER server(ssid, password, serverUrl);

void setup() {
  Serial.begin(9600);
}


void loop() {
  
 

  conditions = server.check_connection();  // Check WiFi connection

   if (conditions) {    
    jsonData = server.get_data();
    
    if (jsonData.length() > 0) {
      // Create barcode list from the received JSON
      barcodes = server.createBarcodeList(jsonData);

      // Send the barcode list if available
      if (!barcodes.empty()) {
        Serial.println("=== Barcode List Start ===");
        for (const String& code : barcodes) {
          Serial.println(code);  
          Serial.flush();
        }
        Serial.println("=== Barcode List End ===");
      } else {
        Serial.println("No barcodes found.");
      }
    }
  }
}








