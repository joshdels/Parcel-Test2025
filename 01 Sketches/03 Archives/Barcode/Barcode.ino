const int barcode_pin = 30;    // Barcode scanner pin
const int solenoid_pin = 32;   // Solenoid control pin
int scanCount = 0;             // Counter for scans
const int maxScans = 5;        // Maximum number of scans

void setup() {
  pinMode(barcode_pin, OUTPUT); // Set barcode pin as output
  pinMode(solenoid_pin, OUTPUT); // Set solenoid pin as output
  Serial.begin(9600);           // Initialize Serial Monitor
  Serial2.begin(9600);          // Initialize Serial2 for barcode scanner
  Serial.println("System Ready...");
}

void loop() {
  scanBarcodes();               // Handle barcode scanning

  if (scanCount >= maxScans) {  // Check if max scans are reached
    activateSolenoid();         // Open solenoid after scanning
    scanCount = 0;              // Reset scan counter if needed
  }
}

// Function to handle barcode scanning
void scanBarcodes() {
  digitalWrite(barcode_pin, HIGH);  // Activate scanner
  delay(20);                        // Short activation delay

  if (Serial2.available()) {        // Check if data is available from scanner
    char c = Serial2.read();        // Read one character
    Serial.print(c);                // Print the character to Serial Monitor

    if (c == '\n') {                // Assuming barcode ends with newline
      scanCount++;                  // Increment scan count
      Serial.print("Scan #");
      Serial.println(scanCount);
    }
  }

  digitalWrite(barcode_pin, LOW);   // Deactivate scanner
  delay(100);                       // Delay to prevent rapid scanning
}

// Function to activate the solenoid
void activateSolenoid() {
  Serial.println("Activating solenoid...");
  digitalWrite(solenoid_pin, HIGH); // Open solenoid
  delay(2000);                      // Keep solenoid open for 2 seconds
  digitalWrite(solenoid_pin, LOW);  // Close solenoid
  Serial.println("Solenoid deactivated.");
}
