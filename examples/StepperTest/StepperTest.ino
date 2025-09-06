// AFMotor R4 Compatible Library - Stepper Motor Test
// Hardware Setup: Stepper motors on M1&M2 (stepper 1)

#include "AFMotor_R4.h"

// Connect a stepper motor with 48 steps per revolution (7.5 degree)
// to motor port #1 (M1 and M2)
AF_Stepper motor(48, 1);

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");
  
  motor.setSpeed(10);  // 10 rpm   
}

void loop() {
  Serial.println("Single coil steps");
  motor.step(100, FORWARD, SINGLE); 
  motor.step(100, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  motor.step(100, FORWARD, DOUBLE); 
  motor.step(100, BACKWARD, DOUBLE);

  Serial.println("Interleaved coil steps");
  motor.step(100, FORWARD, INTERLEAVE); 
  motor.step(100, BACKWARD, INTERLEAVE); 

  Serial.println("Microsteps");
  motor.step(50, FORWARD, MICROSTEP); 
  motor.step(50, BACKWARD, MICROSTEP);
}
