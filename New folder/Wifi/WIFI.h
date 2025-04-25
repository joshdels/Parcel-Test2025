#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> 

class WIFI {
  private:
    const char* ssid;
    const char* password;
    const char* serverUrl;
    WiFiClient client;  
    HTTPClient http;    

  public:
    WIFI(const char* ssid, const char* password, const char* serverUrl);

    bool check_connection();

    String get_data();

    void send_data();

    String deserialize_data(String payload);

};

#endif

