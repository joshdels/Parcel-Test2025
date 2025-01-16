//Parcel_Barcode_Scanner_v.10

const int barcode_pin = 31;    // Barcode scanner control pin
String lastBarcode = "";       // Store the last scanned barcode

void setup() {
  pinMode(barcode_pin, OUTPUT);  // Set barcode pin as output
  Serial.begin(9600);            // Initialize Serial Monitor (for debug output)
  Serial2.begin(9600);           // Initialize Serial2 for barcode scanner communication
  Serial.println("System Ready...");
}

void loop() {
  scanBarcodes(); // Handle barcode scanning continuously
}

// Function to handle barcode scanning
void scanBarcodes() {
  digitalWrite(barcode_pin, HIGH);  // Activate barcode scanner
  delay(20);                        // Short activation delay

  if (Serial2.available()) {        // Check if data is available from the scanner (Serial2)
    String barcode = "";
    while (Serial2.available()) {
      char c = Serial2.read();
      if (c == '\n') break;         // Stop reading at newline (end of barcode)
      barcode += c;                 // Append character to barcode string
    }

    // Check if the barcode is new
    if (barcode.length() > 0 && barcode != lastBarcode) {
      lastBarcode = barcode;        // Update last scanned barcode
      Serial.print("Scanned Barcode: ");
      Serial.println(barcode);
    }
  }

  digitalWrite(barcode_pin, LOW); 
  delay(5000);                     
}
