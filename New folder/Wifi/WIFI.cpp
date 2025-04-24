#include "WIFI.h"

WIFI::WIFI(const char* ssid, const char* password) {
  this->ssid = ssid;
  this->password = password;
}

void WIFI::check_connection() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi Successfully");
  Serial.println(WiFi.localIP());
}

// Function to fetch data from the server
void get_data() {
  http.begin(client, serverUrl);
  int httpCode = http.GET();  // Perform the GET request

  Serial.print("HTTP Response code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {  // Check if the request was successful
    String payload = http.getString();  // Get the response payload
    Serial.println("Received data:");
    Serial.println(payload);

//       /*// Parse JSON
//       DynamicJsonDocument doc(1024);
//       DeserializationError error = deserializeJson(doc, payload);

//       if (!error) {
//         JsonArray posts = doc["posts"].as<JsonArray>();
//         for (JsonObject post : posts) {
//           int id = post["id"];
//           const char* title = post["title"];
//           const char* author = post["author"];
//           const char* date = post["date"];

//           // Print out the JSON content
//           Serial.print("ID: ");
//           Serial.println(id);
//           Serial.print("Title: ");
//           Serial.println(title);
//           Serial.print("Author: ");
//           Serial.println(author);
//           Serial.print("Date: ");
//           Serial.println(date);
//           Serial.println();
//         }
//       } else {
//         Serial.print("JSON Parsing Error: ");
//         Serial.println(error.c_str()); 
//       }*/

//     } else {
//       Serial.println("Error in HTTP request");
//     }

//     http.end();  // Free resources
//   } else {
//     Serial.println("WiFi is not connected!");
//   }
// }


void send_data() {
  http.begin(client, serverUrl);  // Begin HTTP request
  http.addHeader("Content-Type", "application/json"); // Set content type

  String postData = R"({
    "id": 5,
    "title": "Post 3",
    "author": "Author 3",
    "date": "2025-01-25",
    "barcode": "zasdfasdf,
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

  http.end();  // Free resources
  } 

}

// //Add Data to existing json
// void append_http_data() {
//   if (WiFi.status() == WL_CONNECTED) {
//     http.begin(client, serverUrl);
//     http.addHeader("Content-Type", "application/json");

//     String postData = R"({ 
//       "id": 1,
//       "title": "Q 5",
//       "author": "asdfasdf 4",
//       "date": "2025-01-25"
//     })";    

//     Serial.println("Sending POST data:");
//     Serial.println(postData);

//     int httpCode = http.POST(postData); // Send POST request
//     Serial.print("HTTP Response code: ");
//     Serial.println(httpCode);

//     if (httpCode == 200 || httpCode == 201) { // Check for successful response
//       String response = http.getString();
//       Serial.println("Server response:");
//       Serial.println(response);
//     } else {
//       Serial.println("Error in POST request or non-200 response");
//     }

//     http.end(); // Free resources
//   } else {
//     Serial.println("WiFi is not connected!");
//   }
// }
