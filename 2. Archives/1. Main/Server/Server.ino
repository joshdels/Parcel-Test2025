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

SERVER server(ssid, password, serverUrl);

// ---------------------------------------------------------- EDITABLE SECTION ---------------------------------

const char* ssid = 'YOUR SSID NAME';
const char* password = 'YOUR PASSWORD';
const char* serverUrl = 'YOUR SERVER URL';


//---------------------------------------------------------------------------------------------------------------

void setup() {
  // Used delimeter to be read via UART Serial3 to be read by ATMEGA 
  Serial.begin(9600); 
  Serial.println("Connecting to WiFi...");

  conditions = server.check_connection();  // Check WiFi connection

   if (conditions) {
    Serial.println("=== WiFi Connected ===");

    // Mark the start of data fetching
    Serial.println("=== Fetching Server Data Start ===");
    delay(100);
    
    jsonData = server.get_data();
    
    if (jsonData.length() > 0) {
      // Create barcode list from the received JSON
      barcodes = server.createBarcodeList(jsonData);

      // Send the barcode list if available
      if (!barcodes.empty()) {
        Serial.println("=== Barcode List Start ===");
        for (const String& code : barcodes) {
          Serial.println(code);  
          delay(300);           
        }
        Serial.println("=== Barcode List End ===");
      } else {
        Serial.println("No barcodes found.");
      }
    } else {
      Serial.println("No data received from server.");
    }

    // Mark the end of data fetching
    Serial.println("=== Fetching Server Data End ===");
    delay(300);
  } else {
    Serial.println("Failed to connect to WiFi.");
  }

  // Mark the end of WiFi connection
  Serial.println("=== WiFi Connection End ===");
}

void loop() {
}




