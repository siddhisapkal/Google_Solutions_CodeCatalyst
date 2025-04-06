#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

const int buttonPin = 13;  // GPIO 13 for SOS button
const int ledPin = 25;     // GPIO 25 for alert LED

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  if (!mpu.begin()) {
    Serial.println("❌ MPU6050 not detected. Check connections!");
    while (1);
  }

  Serial.println("✅ MPU6050 Found!");
  delay(100);
}

void loop() {
  // Check button for manual SOS
  if (digitalRead(buttonPin) == LOW) {
    digitalWrite(ledPin, HIGH);
    Serial.println("🚨 Manual SOS Triggered!");
    Serial.println("📍 Location: Lat 18.5204° N, Long 73.8567° E");
    delay(1000);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Read accelerometer data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accX = a.acceleration.x;
  float accY = a.acceleration.y;
  float accZ = a.acceleration.z;

  // Fall Detection Logic
  if (abs(accX) > 20 || abs(accY) > 20 || abs(accZ) > 25) {
    digitalWrite(ledPin, HIGH);
    Serial.println("🚨 SOS: Sudden Fall Detected!");
    Serial.println("📍 Location: Lat 18.5204° N, Long 73.8567° E");
    delay(2000); // Debounce delay
  }
}
