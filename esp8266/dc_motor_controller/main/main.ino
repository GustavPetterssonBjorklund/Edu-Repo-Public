#include "dc_motor_controller.h"
/**
OBS! Please read this documentation carefully

Pins (using default for the dc_motor_controller library):
  - ENA: D1 (GPIO5) for motor speed control (PWM)
  - IN1: D3 (GPIO0) for motor direction control
  - IN2: D4 (GPIO2) for motor direction control
  - Potentiometer: A0 for speed input

Motor direction can be changed by changing the motor_dir bool
*/

motorController mc;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int potValue = 0;
  
  // Smooth the potentiometer readings
  for (int i = 0; i < 10; i++) {
    potValue += analogRead(potPin);
    delay(10);
  }
  potValue /= 10;

  Serial.println(potValue); // Debug: Print smoothed potentiometer value

  // Map the potentiometer value to PWM (0–255)
  int motorSpeed = map(potValue, 0, 1023, 0, 255);

  mc.setSpeed(motorSpeed);
}
