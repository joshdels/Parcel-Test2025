// Pins setup
#define trig_pin 36
#define echo_pin 34

// Variables to store
float duration;
float distance;
const float min_distance = 5.00;  // Minimum distance in cm
const int num_samples = 5;        // Number of samples for averaging
bool stop_operation = false;      // Flag to indicate if the operation should stop

void setup() {
  // Configure pins
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);

  // Start serial communication
  Serial.begin(9600);
  Serial.println("Proximity sensor is ready, Josh!");
}

float get_distance() {
  float total_distance = 0;

  // Take multiple readings for averaging
  for (int i = 0; i < num_samples; i++) {
    // Ensure the trig_pin is LOW
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(5);

    // Send a 10-microsecond pulse to trigger the sensor
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);

    // Measure the duration of the echo in microseconds
    duration = pulseIn(echo_pin, HIGH);

    // Calculate the distance in cm
    float single_distance = (duration * 0.0343) / 2;

    // Accumulate valid readings only
    if (single_distance > 2 && single_distance < 400) {
      total_distance += single_distance;
    } else {
      // Retry if an invalid reading is encountered
      i--;
    }

    // Small delay between samples
    delay(10);
  }

  // Return the average distance
  return total_distance / num_samples;
}


  // Print the distance
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Wait before the next reading
  delay(1000);
}

void loop() {
  // Run the proximity sensor logic continuously
  run_proximity_sensor();
}
