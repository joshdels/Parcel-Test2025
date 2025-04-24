#include "BARCODE.h"

//hay nako kataw anan, baliktan ang high og low sa scanner,, anyways

const int barcodePin = 4;
const int txPin = 2; //red outside
const int rxPin = 3; //yellow inside

BARCODE scanner(barcodePin, txPin, rxPin);  // Correct object initialization


void setup() {
  Serial.begin(9600);
  delay(5000);// this works!
  String scanned_barcodes = scanner.scanBarcodes();
  Serial.println("\nVoid First");
  Serial.println(scanned_barcodes);
  Serial.println("Void End");

  
}

void loop(){
  // scanner.on_off();
  // scanner.scanBarcodes();
  // String barcodes = scanner.scanBarcodes();
  // Serial.println(barcodes);   
  // scanner.scanBarcodes();

}


