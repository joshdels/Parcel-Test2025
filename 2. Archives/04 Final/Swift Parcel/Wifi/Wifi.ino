#include "WIFI.h"

const char* ssid = "SSD ID";
const char* password = "PASSWORD";
const char* serverUrl = "URL";

bool conditions = false;

String jsonData = "";
String weather = "";

WIFI wifi(ssid, password, serverUrl);



void setup() {
  Serial.begin(115200);

  conditions = wifi.check_connection();
  if (conditions) {
    jsonData = wifi.get_data();
    // weather = wifi.deserialize_data(jsonData);
    Serial.println(jsonData);
    // Serial.println(weather);
  }
  // wifi.send_data();
}

void loop(){
//   if (conditions){
//      Serial.println(jsonData);
//      delay(5000);
//   }
}


  // if (wifi.check_connection()) {
  //   String jsonData = wifi.get_data();
  //   // wifi.deserialize_data(jsonData);
  //   Serial.print(jsonData);
  // } else {
  //   String jsonData = wifi.get_data();
  //   Serial.print(jsonData);
  // };



