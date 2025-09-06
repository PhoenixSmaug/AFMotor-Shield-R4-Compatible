// AFMotor R4 Compatible Library - Multiple Stepper Motor Test
// Hardware Setup: Stepper motors on M1&M2 (stepper 1) and M3&M4 (stepper 2)

#include "AFMotor_R4.h"

// Two steppers, one on port 1 (M1&M2) and one on port 2 (M3&M4)
AF_Stepper stepper1(48, 1);
AF_Stepper stepper2(48, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Multiple Stepper test!");
  
  stepper1.setSpeed(30);  // 30 rpm   
  stepper2.setSpeed(20);  // 20 rpm
}

void loop() {
  Serial.println("Stepper 1 forward");
  stepper1.step(100, FORWARD, SINGLE);
  
  Serial.println("Stepper 2 forward");  
  stepper2.step(100, FORWARD, SINGLE);
  
  Serial.println("Stepper 1 backward");
  stepper1.step(100, BACKWARD, SINGLE);
  
  Serial.println("Stepper 2 backward");
  stepper2.step(100, BACKWARD, SINGLE);
  
  Serial.println("Both steppers forward");
  // Note: This runs them sequentially, not simultaneously
  stepper1.step(50, FORWARD, DOUBLE);
  stepper2.step(50, FORWARD, DOUBLE);
  
  delay(1000);
}
