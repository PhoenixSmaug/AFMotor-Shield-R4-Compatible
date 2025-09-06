/* Complete Motor Example for Arduino R4 WiFi
 * Demonstrates both DC motor and stepper motor functionality
 * using the AFMotor_R4_Compatible library
 * 
 * Hardware setup:
 * - Arduino R4 WiFi with Adafruit Motor Shield v1
 * - DC motors connected to M1, M2, M3, M4
 * - Stepper motors can use M1&M2 (stepper 1) or M3&M4 (stepper 2)
 */

#include "AFMotor_R4.h"

// Create DC motor objects (compatible with original AFMotor syntax)
AF_DCMotor motor1(1);  // DC motor on M1
AF_DCMotor motor2(2);  // DC motor on M2
AF_DCMotor motor3(3);  // DC motor on M3
AF_DCMotor motor4(4);  // DC motor on M4

// Create stepper motor objects
AF_Stepper stepper1(200, 1);  // 200 steps per revolution, uses M1&M2
AF_Stepper stepper2(200, 2);  // 200 steps per revolution, uses M3&M4

void setup() {
  Serial.begin(9600);
  Serial.println("AFMotor R4 Compatible Library Test");
  Serial.println("==================================");
  
  // Set stepper speeds (RPM)
  stepper1.setSpeed(10);  // 10 RPM
  stepper2.setSpeed(20);  // 20 RPM
  
  // Initialize DC motors
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor4.setSpeed(150);
  
  // Release all motors initially
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  stepper1.release();
  stepper2.release();
  
  delay(1000);
}

void loop() {
  // Test DC Motors
  testDCMotors();
  delay(2000);
  
  // Test Stepper Motors
  testStepperMotors();
  delay(2000);
  
  // Test Mixed Operation (DC + Stepper)
  testMixedOperation();
  delay(2000);
}

void testDCMotors() {
  Serial.println("Testing DC Motors");
  Serial.println("-----------------");
  
  // Test forward direction with speed ramp
  Serial.println("DC Motors Forward - Speed Ramp Up");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  
  for (int speed = 0; speed <= 255; speed += 5) {
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
    delay(50);
  }
  
  // Speed ramp down
  Serial.println("DC Motors Forward - Speed Ramp Down");
  for (int speed = 255; speed >= 0; speed -= 5) {
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
    delay(50);
  }
  
  // Test backward direction
  Serial.println("DC Motors Backward");
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  
  for (int speed = 0; speed <= 200; speed += 10) {
    motor1.setSpeed(speed);
    motor2.setSpeed(speed);
    motor3.setSpeed(speed);
    motor4.setSpeed(speed);
    delay(100);
  }
  
  // Test brake
  Serial.println("DC Motors Brake");
  motor1.run(BRAKE);
  motor2.run(BRAKE);
  motor3.run(BRAKE);
  motor4.run(BRAKE);
  delay(1000);
  
  // Release motors
  Serial.println("DC Motors Release");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void testStepperMotors() {
  Serial.println("Testing Stepper Motors");
  Serial.println("----------------------");
  
  // Test different stepping styles
  Serial.println("Stepper 1: Single Steps Forward");
  stepper1.step(100, FORWARD, SINGLE);
  
  Serial.println("Stepper 1: Double Steps Backward");
  stepper1.step(100, BACKWARD, DOUBLE);
  
  Serial.println("Stepper 2: Interleave Steps Forward");
  stepper2.step(50, FORWARD, INTERLEAVE);
  
  Serial.println("Stepper 2: Microsteps Backward");
  stepper2.step(25, BACKWARD, MICROSTEP);
  
  // Test both steppers simultaneously
  Serial.println("Both Steppers: Alternating Steps");
  for (int i = 0; i < 5; i++) {
    stepper1.step(20, FORWARD, SINGLE);
    stepper2.step(20, BACKWARD, SINGLE);
  }
  
  // Release steppers
  stepper1.release();
  stepper2.release();
}

void testMixedOperation() {
  Serial.println("Testing Mixed Operation (DC + Stepper)");
  Serial.println("--------------------------------------");
  
  // Note: Stepper 1 uses M1&M2, Stepper 2 uses M3&M4
  // So we can't run DC motors and steppers on the same channels simultaneously
  // But we can demonstrate sequential operation
  
  Serial.println("DC Motors M1&M2 Forward");
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor1.setSpeed(150);
  motor2.setSpeed(150);
  delay(2000);
  
  Serial.println("Switch to Stepper 1 (M1&M2)");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(100);
  
  stepper1.step(50, FORWARD, SINGLE);
  stepper1.release();
  
  Serial.println("DC Motors M3&M4 Backward");
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
  delay(2000);
  
  Serial.println("Switch to Stepper 2 (M3&M4)");
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(100);
  
  stepper2.step(50, BACKWARD, DOUBLE);
  stepper2.release();
}

// Additional utility functions for advanced usage

void stepperSpeedTest() {
  Serial.println("Stepper Speed Test");
  Serial.println("------------------");
  
  // Test different speeds
  int speeds[] = {5, 10, 20, 30, 50};
  
  for (int i = 0; i < 5; i++) {
    Serial.print("Testing speed: ");
    Serial.print(speeds[i]);
    Serial.println(" RPM");
    
    stepper1.setSpeed(speeds[i]);
    stepper1.step(50, FORWARD, SINGLE);
    delay(500);
  }
  
  stepper1.release();
}

void motorDirectionTest() {
  Serial.println("Motor Direction Test");
  Serial.println("--------------------");
  
  // Test all possible combinations
  uint8_t directions[] = {FORWARD, BACKWARD, BRAKE, RELEASE};
  String dirNames[] = {"FORWARD", "BACKWARD", "BRAKE", "RELEASE"};
  
  for (int i = 0; i < 4; i++) {
    Serial.print("Testing direction: ");
    Serial.println(dirNames[i]);
    
    motor1.run(directions[i]);
    motor1.setSpeed(180);
    delay(1000);
  }
}

/* Usage Examples:
 * 
 * Basic DC Motor:
 * AF_DCMotor motor(1);
 * motor.setSpeed(255);
 * motor.run(FORWARD);
 * 
 * Basic Stepper:
 * AF_Stepper stepper(200, 1);
 * stepper.setSpeed(10);
 * stepper.step(100, FORWARD, SINGLE);
 * 
 * Available DC Motor Commands:
 * - FORWARD: Motor spins forward
 * - BACKWARD: Motor spins backward  
 * - BRAKE: Motor brakes (both leads high)
 * - RELEASE: Motor stops (both leads low)
 * 
 * Available Stepper Styles:
 * - SINGLE: Single coil activation (normal stepping)
 * - DOUBLE: Double coil activation (more torque)
 * - INTERLEAVE: Interleaved single/double (twice the resolution)
 * - MICROSTEP: Microstepping (16x resolution, smooth motion)
 * 
 * Motor Shield Connections:
 * - M1: PWM on pin 11, direction controlled by shift register bits 2&3
 * - M2: PWM on pin 3, direction controlled by shift register bits 1&4
 * - M3: PWM on pin 6, direction controlled by shift register bits 5&7
 * - M4: PWM on pin 5, direction controlled by shift register bits 0&6
 * - Stepper 1: Uses M1&M2 connections
 * - Stepper 2: Uses M3&M4 connections
 */
