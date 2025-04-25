#include "WIFI.h"

WIFI::WIFI(const char* ssid, const char* password, const char* serverUrl) {
  this->ssid = ssid;
  this->password = password;
  this->serverUrl = serverUrl;
}

bool WIFI::check_connection() {

  int attempt = 0;
  const int maxAttempts = 20;

  WiFi.begin(ssid, password);

  delay(1000);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED && attempt < maxAttempts) {
    delay(1000);
    Serial.print(".");
    attempt++;
  }
  if (WiFi.status() == WL_CONNECTED){
    delay(1000);
    Serial.println("\nConnected to WiFi Successfully");
    Serial.println(WiFi.localIP());
    return true;
  }
  else {
    Serial.println("\nFailed to connect to WiFi :(\nPlease Check your WiFi SSID or Password");
    return false;
  }
}

String WIFI::get_data() {
  Serial.println("Getting the data from the server");
  http.begin(client, serverUrl);
  int httpCode = http.GET(); 

  Serial.println("HTTP Response code: ");
  Serial.print(httpCode);

  String payload = "";

  if (httpCode == 200) { 
    String payload = http.getString(); 
    Serial.println("Received data:");
    Serial.println(payload);
    return payload;
  } 
  else {
    Serial.println("Error in HTTP request\nPlease check serverUrl\n");
  }
  http.end();
  return payload;
}

String WIFI::deserialize_data(String payload){
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);

  //need to be careful here
  const char* weatherMain = doc["weather"][0]["main"];
  Serial.print("Weather: ");
  Serial.println(weatherMain);

  return String(weatherMain);
}

void WIFI::send_data() {
  http.begin(client, serverUrl);  // Begin HTTP request
  http.addHeader("Content-Type", "application/json"); // Set content type

  String postData = R"({
    "id": 5,
    "title": "Post 3",
    "author": "Author 3",
    "date": "2025-01-25",
    "barcode": "zasdfasdf"
  })";

  Serial.println("Sending POST data:");
  Serial.println(postData);

  int httpCode = http.POST(postData);  // Perform the POST request
  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);

  if (httpCode == 200 || httpCode == 201) {  // Check for successful response
    String response = http.getString();  // Get the response payload
    Serial.println("Server response:");
    Serial.println(response);
  } else {
    Serial.println("Error in POST request or non-200 response");
  }
  http.end();  
} 

