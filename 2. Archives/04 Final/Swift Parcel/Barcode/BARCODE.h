#ifndef BARCODE_H
#define BARCODE_H

#include "Arduino.h"

class BARCODE {
  private:
    int barcodePin;
    String lastBarcode;

  public:
    BARCODE(int barcodePin);

    String scanBarcodes();
};

#endif