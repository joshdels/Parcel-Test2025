#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>  // For JSON parsing

// Wi-Fi credentials
const char* ssid = "PLDTHOMEFIBRiEcjt";
const char* password = "PLDTWIFInC3i3";

IPAddress local_ip(192, 168, 1, 21);  // Change this IP to your desired one
IPAddress gateway(192, 168, 1, 1);     // Gateway (router IP)
IPAddress subnet(255, 255, 255, 0);    // Subnet mask

// Replace this with your computer's local IP address
const char* serverUrl = "http://192.168.1.12:8080/data.json";  

void setup() {
  // Start the Serial Monitor
  
  Serial.begin(115200);


  // Connect to Wi-Fi
  WiFi.config(local_ip, gateway, subnet);  // Set static IP
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  //Connected to WiFi
  Serial.println("Connected to WiFi!");
  Serial.print("ESP IP Address: ");
  Serial.println(WiFi.localIP());

  // Fetch and parse JSON data from the server
  HTTPClient http;
  WiFiClient client;
  http.begin(client, serverUrl);  // HTTP request to the server

  int httpCode = http.GET();  // Make the request
  Serial.print("HTTP Response code:");
  Serial.print(httpCode);

  delay(500);
  if (httpCode == 200) {
    // If the request is successful, parse the JSON data
    String payload = http.getString();
    Serial.println("Received data:");

    // Create a JSON document
    DynamicJsonDocument doc(1024);  // Create a document of 1KB size
    deserializeJson(doc, payload);  // Parse the payload into the JSON document

    // Extract data from the JSON
    JsonArray posts = doc["posts"].as<JsonArray>();
    for (JsonObject post : posts) {
      int id = post["id"];
      const char* title = post["title"];
      const char* author = post["author"];
      const char* date = post["date"];

      // Print out the JSON content
      Serial.print("ID: ");
      Serial.println(id);
      Serial.print("Title: ");
      Serial.println(title);
      Serial.print("Author: ");
      Serial.println(author);
      Serial.print("Date: ");
      Serial.println(date);
      Serial.println();
    }
  } else {
    // If the HTTP request failed
    Serial.println("Error in HTTP request");
  }

  http.end();  // Free resources
}

void loop() {
  // Add your main code here
}

