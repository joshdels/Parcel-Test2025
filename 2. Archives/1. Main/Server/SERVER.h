#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h> 
#include <ArduinoJson.h> 
#include <vector>

class SERVER {
  private:
    const char* ssid;
    const char* password;
    const char* serverUrl;
    WiFiClient client; 
    // WiFiClientSecure client; // activate this for example "https://"
    HTTPClient http;   

  public:
    
    SERVER(const char* ssid, const char* password, const char* serverUrl);

    bool check_connection();
    String get_data();
    std::vector<String> createBarcodeList(String payload);
    
};

#endif

