#include <Servo.h>

// Pin Definitions
#define TDS_PIN A1        // TDS Sensor Signal
#define RELAY_PIN 3       // Relay Control Pin
#define SERVO_PIN 9       // Servo Control Pin

// System Settings
const int TDS_THRESHOLD = 800; // Water contamination threshold in PPM
const int SERVO_CLEAN_POS = 0; // Gate angle for safe water
const int SERVO_DIRTY_POS = 90; // Gate angle for contaminated water

Servo waterGate;

void setup() {
  // Start Serial Communication for Laptop Telemetry
  Serial.begin(9600);

  // Configure Relay Pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Turn pump ON (Active LOW/HIGH depending on relay module)

  // Attach and initialize Servo Motor
  waterGate.attach(SERVO_PIN);
  waterGate.write(SERVO_CLEAN_POS); // Set default position to clean path

  Serial.println("==========================================");
  Serial.println(" AUTOMATED WATER PURIFICATION & TELEMETRY ");
  Serial.println("==========================================");
  delay(1000);
}

void loop() {
  // Read raw analog value from TDS sensor
  int analogValue = analogRead(TDS_PIN);
  
  // Convert analog reading (0-1023) to voltage (0-5V)
  float voltage = analogValue * (5.0 / 1023.0);
  
  // Calculate approximate TDS in PPM (Parts Per Million)
  float tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5;

  // Print live telemetry data to Laptop Serial Monitor / Serial Plotter
  Serial.print("Voltage: ");
  Serial.print(voltage, 2);
  Serial.print("V | TDS: ");
  Serial.print(tdsValue, 0);
  Serial.print(" PPM | Status: ");

  // Decision Logic
  if (tdsValue <= TDS_THRESHOLD) {
    // Water is clean
    Serial.println("SAFE (Routing to Clean Output)");
    waterGate.write(SERVO_CLEAN_POS); // Route to clean valve
  } else {
    // Water is contaminated
    Serial.println("CONTAMINATED (Diverting to Reject)");
    waterGate.write(SERVO_DIRTY_POS); // Divert to reject valve
  }

  // Update telemetry readings every 1 second
  delay(1000);
}