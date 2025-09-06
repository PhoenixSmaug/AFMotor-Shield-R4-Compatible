/* AFMotor_R4.h - Header file for Arduino R4 WiFi compatible AFMotor library
 * Compatible replacement for the Adafruit Motor Shield V1 library
 * Supports both DC motors and stepper motors
 * 
 * This library replicates the original AFMotor library API while being
 * compatible with Arduino R4 WiFi and other non-AVR boards.
 * 
 * Original AFMotor library: https://github.com/adafruit/Adafruit-Motor-Shield-library
 * Inspired by: https://forum.arduino.cc/t/using-l293d-motor-driver-shield-without-any-library/685601
 * 
 * Usage:
 * #include "AFMotor_R4.h"
 * AF_DCMotor motor1(1);        // Create DC motor on M1
 * AF_Stepper stepper(200, 1);  // Create stepper motor (200 steps/rev) on M1&M2
 */

#ifndef AFMotor_R4_h
#define AFMotor_R4_h

#include "Arduino.h"

// Pin definitions for the motor shield
#define PWM2B 3      // PWM for M1 & M2 motors
#define DIR_CLK 4    // SHCP of 74HC595 shift register
#define PWM0B 5      // PWM for M3 & M4 motors  
#define PWM0A 6      // PWM for M3 & M4 motors
#define DIR_EN 7     // OE (Output Enable) of 74HC595
#define DIR_SER 8    // DS (Data Serial) of 74HC595
#define PWM1A 9      // PWM for servos (not used for DC motors)
#define PWM1B 10     // PWM for servos (not used for DC motors)
#define PWM2A 11     // PWM for M1 & M2 motors
#define DIR_LATCH 12 // STCP (Storage Register Clock) of 74HC595

// Bit positions in the 74HC595 shift register output
#define MOTOR1_A 2   // Q2
#define MOTOR1_B 3   // Q3
#define MOTOR2_A 1   // Q1
#define MOTOR2_B 4   // Q4
#define MOTOR3_A 5   // Q5
#define MOTOR3_B 7   // Q7
#define MOTOR4_A 0   // Q0
#define MOTOR4_B 6   // Q6

// Motor direction constants
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

// Stepper motor style constants
#define SINGLE 1
#define DOUBLE 2
#define INTERLEAVE 3
#define MICROSTEP 4

// Microstep configuration
#define MICROSTEPS 16

// Global variables (extern declarations)
extern uint8_t latch_state;
extern bool motor_controller_initialized;
extern uint8_t microstepcurve[];

// Function declarations
void latch_tx();
void initMotorController();

// DC Motor class
class AF_DCMotor {
private:
  uint8_t motornum;
  uint8_t pwm_pin;
  uint8_t motor_a_bit;
  uint8_t motor_b_bit;

public:
  AF_DCMotor(uint8_t num, uint8_t freq = 0);  // freq parameter for compatibility, not used
  void run(uint8_t cmd);
  void setSpeed(uint8_t speed);
};

// Stepper Motor class
class AF_Stepper {
private:
  uint8_t steppernum;
  uint16_t revsteps;
  uint8_t currentstep;
  uint32_t usperstep;
  uint32_t steppingcounter;

public:
  AF_Stepper(uint16_t steps, uint8_t num);
  void setSpeed(uint16_t rpm);
  void release(void);
  void step(uint16_t steps, uint8_t dir, uint8_t style = SINGLE);
  uint8_t onestep(uint8_t dir, uint8_t style);
};

#endif // AFMotor_R4_h
