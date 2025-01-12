#define solenoid_pin 32
#define barcode_pin 30
int scanCount = 0;
int maxScans = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(solenoid_pin, OUTPUT);
  pinMode(barcode_pin, OUTPUT);
  Serial.begin(9600);
  Serial2.begin(9600);


}

void barcode_scanner(){
  digitalWrite(barcode_pin, HIGH);
  delay(20);

  if (Serial2.available()){
    char c = Serial2.read();
    Serial.print(c);
    digitalWrite(solenoid_pin, HIGH);
    delay(5000);
    scanCount++;
 
  }
}

void solenoid_locker(){
  // digitalWrite(solenoid_pin, HIGH);
  // delay(5000);
  digitalWrite(solenoid_pin, LOW);
  delay(1000);
}


void loop() {
  // put your main code here, to run repeatedly:
barcode_scanner();
  if (scanCount >= maxScans) {
    scanCount = 0;
    digitalWrite(barcode_pin, LOW);
    solenoid_locker();
    delay(20);
  }
  
}
