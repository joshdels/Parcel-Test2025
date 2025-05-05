#include "BARCODE.h"

const int barcodePin = 13;
const int txPin = 10; //red outside
const int rxPin = 11; //yellow inside

BARCODE scanner(barcodePin, txPin, rxPin);  // Correct object initialization


void setup() {
  String barcodes = scanner.scanBarcodes();
  // Only print if barcode is not empty
  if (barcodes.length() > 0) {
    Serial.print("Scanned Barcode: ");
    Serial.println(barcodes);
  } else {
    Serial.println("No barcode detected.");
  }

}

void loop(){
  // String barcodes = scanner.scanBarcodes();
  // Serial.println(barcodes);   

}


