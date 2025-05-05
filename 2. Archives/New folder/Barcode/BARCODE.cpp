#include "BARCODE.h"
#include "Arduino.h"

BARCODE::BARCODE(int barcodePin, int txPin, int rxPin)
  : barcodeSerial(txPin, rxPin)
{
  this->barcodePin = barcodePin;  // Set the barcodePin
  this->txPin = txPin;            // Set the txPin
  this->rxPin = rxPin;            // Set the rxPin

  pinMode(barcodePin, OUTPUT);
  barcodeSerial.begin(9600);
  lastBarcode = "";

}

String BARCODE::scanBarcodes() {
  Serial.print("Scanning");
  String barcode = "";

  digitalWrite(barcodePin, HIGH);  // Activate barcode scanner

  // if (barcodeSerial.available()) {  // Check if data is available from the scanner (SoftwareSerial)
  //   while (barcodeSerial.available()) {
  //     char c = barcodeSerial.read();
  //     if (c == '\n') break;       
  //     barcode += c;                 
  //   }

    delay(200);
    while (barcodeSerial.available()) {
      char c = barcodeSerial.read();
      if (c == '\n') break;       
      barcode += c;                 
    }
    delay(200);
    digitalWrite(barcodePin, LOW);  // Deactivate barcode scanner

    // Check if the barcode is new
    if (barcode.length() > 0 && barcode != lastBarcode) {
      lastBarcode = barcode;      
      Serial.print("Scanned Barcode: ");
      Serial.println(barcode);      
    }

  delay(5000);                     // Delay for 5 seconds before scanning again
  return barcode;
  
}