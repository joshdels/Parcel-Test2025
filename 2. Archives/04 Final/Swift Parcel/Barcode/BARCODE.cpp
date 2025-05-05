#include "BARCODE.h"
#include "Arduino.h"

BARCODE::BARCODE(int barcodePin)
{
  this->barcodePin = barcodePin;            

  pinMode(barcodePin, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600);
  lastBarcode = "";

}

String BARCODE::scanBarcodes() {
  Serial.print("Scanning ");
  String barcode = "";
  
  digitalWrite(barcodePin, HIGH);  // Activate barcode scanner

  // if (barcodeSerial.available()) {  // Check if data is available from the scanner (SoftwareSerial)
  //   while (barcodeSerial.available()) {
  //     char c = barcodeSerial.read();
  //     if (c == '\n') break;       
  //     barcode += c;                 
  //   }

    delay(200);
    while (Serial2.available()) {
      char c = Serial2.read();
      if (c == '\n') break;       
      barcode += c;      
      // Serial.println(barcode);        
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
  Serial.println(barcode);

  return barcode;
  
}