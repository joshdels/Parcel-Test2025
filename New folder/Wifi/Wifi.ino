#include "WIFI.h"

const char* ssid = "PLDTHOMEFIBRiEcjt";
const char* password = "PLDTWIFInC3i3";

WIFI wifi(ssid, password);

void setup() {
  wifi.check_connection();
}

void loop(){
  
}

