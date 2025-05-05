#ifndef BARCODE_H
#define BARCODE_H

#include "Arduino.h"
#include "SoftwareSerial.h"

class BARCODE {
  private:
    int txPin;
    int rxPin;
    int barcodePin;
    SoftwareSerial barcodeSerial;
    String lastBarcode;

  public:
    BARCODE(int barcodePin, int txPin, int rxPin);

    String scanBarcodes();
};

#endif