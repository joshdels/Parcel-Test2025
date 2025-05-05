#include "WIFI.h"

const char* ssid = "PLDTHOMEFIBRiEcjt";
const char* password = "PLDTWIFInC3i3";
const char* serverUrl = "http://api.openweathermap.org/data/2.5/weather?lat=44.34&lon=10.99&appid=97d67a6af77a4f2f42e0312574a5100e";

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

