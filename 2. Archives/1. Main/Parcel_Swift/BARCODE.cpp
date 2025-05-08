#include "BARCODE.h"
#include "Arduino.h"

BARCODE::BARCODE(int barcodePin){
  this->barcodePin = barcodePin;            
  pinMode(barcodePin, OUTPUT);
  Serial2.begin(9600); //UART dont modify serial2
  digitalWrite(barcodePin, HIGH); 
  lastBarcode = "";
}

String BARCODE::activateScanner() {
  //Scans the parcel and return the string of the barcode
  //note that the digitalWrite configuration is inverted its weird :) ,
    //LOW --> scanning
    //HIGH --> turning off
  Serial.println("Scanning");
  String barcode = "";
  
  digitalWrite(barcodePin, LOW);
    delay(800);

    while (Serial2.available()) {
      char c = Serial2.read();
      if (c == '\n') break;       
      barcode += c;          
    }
    delay(200);
    digitalWrite(barcodePin, HIGH);

    if (barcode.length() > 0 && barcode != lastBarcode) {
      lastBarcode = barcode;       
    }
  delay(2000);
  Serial.println(barcode);
  return barcode;
}

