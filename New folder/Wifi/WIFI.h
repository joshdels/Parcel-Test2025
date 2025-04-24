#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// #include <ArduinoJson.h>  // For JSON parsing

class WIFI {
  private:
    const char* ssid;
    const char* password;
    WiFiClient client;  // Global WiFi client for HTTP
    HTTPClient http;    // Global HTTP client

  public:
    WIFI(const char* ssid, const char* password);

    void check_connection();

    void get_data();

    void send_data();

};

#endif

