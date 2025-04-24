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

}

String BARCODE::scanBarcodes() {
  Serial.println("Scanning");
  String barcode = "";

  
  digitalWrite(barcodePin, LOW);  

  unsigned long startTime = millis();  
  while (millis() - startTime < 3000) { 
    if (barcodeSerial.available()) {
      char c = barcodeSerial.read();
      if (c == '\n') break; 
      barcode += c;  
    }
  }
  
    digitalWrite(barcodePin, HIGH); //baliktad! 

    if (barcode.length() == 0) {
      Serial.println("No barcode found!");
    } else {
      Serial.println("Barcode scanned: ");
      Serial.println(barcode);
    }
    
    return barcode;
  
}


void BARCODE::on_off() {
  //for debugging
  Serial.println("Now Running ON");
  digitalWrite(barcodePin, HIGH);
  delay(3000);
  Serial.println("Now Running OFF");
  digitalWrite(barcodePin, LOW);
  delay(3000);
}