/**
MotorController v.1
Usage steps :
  include class header file:
    #include "dc_motor_controller.h"

  create instance of class:
    motorController mc;

  set speed: 
    mc.setSpeed(<int>);
*/


class motorController {
  private:
    // Directions to abstract the motor direction
    static const bool clockwise = 1;
    static const bool counterclockwise = 0;

    bool motor_dir; 

    int ENA; // Motor speed control (PWM)
    int IN1; // Motor direction control
    int IN2; // Motor direction control

  public:
    // Constructor to initialize motor pins and optionally set direction
    motorController(int enaPin = 5, int in1Pin = 1, int in2Pin = 0, bool direction = clockwise) 
      : ENA(enaPin), IN1(in1Pin), IN2(in2Pin), motor_dir(direction) {
      pinMode(ENA, OUTPUT);
      pinMode(IN1, OUTPUT);
      pinMode(IN2, OUTPUT);

      // Set initial direction
      setDirection(motor_dir);
    }


    // Set motor direction
    void setDirection(bool direction) {
      motor_dir = direction;
      if (motor_dir == clockwise) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
      } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
      }
    }

    // Set motor speed (0-255)
    void setSpeed(int speed) {
      speed = constrain(speed, 0, 255); // Ensure speed is in range
    analogWrite(ENA, speed);
    }

    // Stop the motor
    void stop() {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 0); 
    }
};
