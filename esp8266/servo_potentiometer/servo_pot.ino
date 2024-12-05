/**
OBS! Please read this documentation carfully

Pins:
  NOTE that the pins used are D1 (GPIO 5) And A0
*/


#include <Servo.h> // Library for ESP8266 servo control


Servo myServo;

// Define pin connections
const int servoPin = 5;  // GPIO5 (D1) for servo control
const int potPin = A0;   // A0 for potentiometer input

void setup() {
  // Attach the servo to the defined pin with custom pulse widths
  myServo.attach(servoPin, 544, 2400);
  Serial.begin(115200);
}

void loop() {
  // Read the potentiometer value (0–1023)
  int potValue = analogRead(potPin);
  Serial.println(potValue);

  // Map the potentiometer value to servo angle (0–180 degrees)
  int angle = map(potValue, 0, 1023, 0, 180);

  // Move the servo to the mapped angle
  myServo.write(angle);

  // Add a small delay for stability
  delay(15);
}
