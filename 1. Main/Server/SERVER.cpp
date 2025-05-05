#include "SERVER.h"

SERVER::SERVER(const char* ssid, const char* password, const char* serverUrl) {
  this->ssid = ssid;
  this->password = password;
  this->serverUrl = serverUrl;
}

bool SERVER::check_connection() {

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
    return true;
  }
  else {
    Serial.println("\nFailed to connect to WiFi :(\nPlease Check your WiFi SSID or Password");
    return false;
  }
}

String SERVER::get_data() {
  //Returns a Json data fecthed from the server
  String payload = "";

  // client.setInsecure(); // activate when uses "https://"

  http.begin(client, serverUrl);
  int httpCode = http.GET(); 

  Serial.print("HTTP response code: ");
  Serial.print(httpCode);

  if (httpCode == 200) { 
    payload = http.getString(); 
  } 
  else {
    Serial.println("Error in HTTP request");
  }

  http.end();
  return payload;
}

std::vector<String> SERVER::createBarcodeList(String payload) {
  std::vector<String> barcodes; 
  // NOTE: dynamic array are only acceptable on ESP boards, but not avaialable to ATMEGA since it only allows static arrays
    // use the payload Json data from the server and returns a array of barcodes
    // use a for loop to access the array of barcodes 

  DynamicJsonDocument doc(2048);
  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    return barcodes;
  }

  JsonArray array = doc["data"].as<JsonArray>();
  if (array.isNull()) {
    Serial.println("JSON is not a valid array.");
    return barcodes;
  }

  for (JsonObject item : array) {
    const char* barcode = item["Item_Barcode"];
    if (barcode) {
      barcodes.push_back(String(barcode));
    }
  }

  return barcodes;
}



