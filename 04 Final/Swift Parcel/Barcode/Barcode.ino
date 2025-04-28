#include "BARCODE.h"

const int barcodePin = 50;

BARCODE scanner(barcodePin); 


void setup() {
  Serial.begin(9600);
  // String barcodes = scanner.scanBarcodes();
  // // Only print if barcode is not empty
  // if (barcodes.length() > 0) {
  //   Serial.print("Scanned Barcode: ");
  //   Serial.println(barcodes);
  // } else {
  //   Serial.println("No barcode detected.");
  // }
}


void loop(){
  // String barcodes = scanner.scanBarcodes();
  // Serial.println(barcodes);  


    String barcodes = scanner.scanBarcodes();
  // Only print if barcode is not empty
  if (barcodes.length() > 0) {
    Serial.print("Scanned Barcode: ");
    Serial.println(barcodes);
  } else {
    Serial.println(" No barcode detected.");
  } 

}


