// AFMotor R4 Compatible Library - Basic Motor Test
// Hardware Setup: DC Motor connected to M1


#include "AFMotor_R4.h"

AF_DCMotor motor(1);  // Create motor on M1

void setup() {
  Serial.begin(9600);           
  Serial.println("Basic Motor Test!");

  // Set initial speed and release motor
  motor.setSpeed(200);
  motor.run(RELEASE);
}

void loop() {
  uint8_t i;
  
  Serial.println("Running forward...");
  
  motor.run(FORWARD);
  for (i = 0; i < 255; i++) {
    motor.setSpeed(i);  
    delay(10);
  }
 
  for (i = 255; i != 0; i--) {
    motor.setSpeed(i);  
    delay(10);
  }
  
  Serial.println("Running backward...");
  
  motor.run(BACKWARD);
  for (i = 0; i < 255; i++) {
    motor.setSpeed(i);  
    delay(10);
  }
 
  for (i = 255; i != 0; i--) {
    motor.setSpeed(i);  
    delay(10);
  }
  
  Serial.println("Braking...");
  motor.run(BRAKE);
  delay(1000);
  
  Serial.println("Releasing...");
  motor.run(RELEASE);
  delay(1000);
}
