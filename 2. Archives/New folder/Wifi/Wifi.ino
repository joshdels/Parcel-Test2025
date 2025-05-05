#include "WIFI.h"

const char* ssid = "SSD NAME";
const char* password = "PASSOWRD";
const char* serverUrl = "URL";

WIFI wifi(ssid, password, serverUrl);

void setup() {
  Serial.begin(9600);
  
  if (wifi.check_connection()) {
    Serial.print("Successfully Connect!");
    String jsonData = wifi.get_data();
    wifi.deserialize_data(jsonData);
  };
  
  // wifi.send_data();
}

void loop(){
  
}

