/* AFMotor_R4.cpp - Implementation for Arduino R4 WiFi compatible AFMotor library
 * Compatible replacement for the Adafruit Motor Shield V1 library
 * 
 * This library replicates the original AFMotor library API while being
 * compatible with Arduino R4 WiFi and other non-AVR boards.
 */

#include "AFMotor_R4.h"

// Global variables
uint8_t latch_state = 0;  // Current state of the shift register
bool motor_controller_initialized = false;

// Microstep curve for smooth stepping
uint8_t microstepcurve[] = {0, 25, 50, 74, 98, 120, 141, 162, 180, 197, 212, 225, 236, 244, 250, 253, 255};

// Function to send data to the shift register
void latch_tx() {
  digitalWrite(DIR_LATCH, LOW);
  shiftOut(DIR_SER, DIR_CLK, MSBFIRST, latch_state);
  digitalWrite(DIR_LATCH, HIGH);
}

// Initialize the motor controller
void initMotorController() {
  if (!motor_controller_initialized) {
    // Initialize control pins for the shift register
    pinMode(DIR_EN, OUTPUT);
    pinMode(DIR_SER, OUTPUT);
    pinMode(DIR_LATCH, OUTPUT);
    pinMode(DIR_CLK, OUTPUT);
    
    // Enable the shift register outputs (active LOW)
    digitalWrite(DIR_EN, LOW);
    
    // Reset latch state
    latch_state = 0;
    latch_tx();
    
    motor_controller_initialized = true;
  }
}

// AF_DCMotor class implementation
AF_DCMotor::AF_DCMotor(uint8_t num, uint8_t freq) {  // freq parameter for compatibility, not used
  motornum = num;
  
  // Initialize motor controller if not done already
  initMotorController();
  
  // Set PWM pins and direction bits based on motor number
  switch(num) {
    case 1:
      pwm_pin = PWM2A;      // Pin 11
      motor_a_bit = MOTOR1_A;
      motor_b_bit = MOTOR1_B;
      break;
    case 2:
      pwm_pin = PWM2B;      // Pin 3
      motor_a_bit = MOTOR2_A;
      motor_b_bit = MOTOR2_B;
      break;
    case 3:
      pwm_pin = PWM0A;      // Pin 6
      motor_a_bit = MOTOR3_A;
      motor_b_bit = MOTOR3_B;
      break;
    case 4:
      pwm_pin = PWM0B;      // Pin 5
      motor_a_bit = MOTOR4_A;
      motor_b_bit = MOTOR4_B;
      break;
    default:
      return;
  }
  
  // Initialize PWM pin
  pinMode(pwm_pin, OUTPUT);
  
  // Set both motor direction bits to 0 initially
  latch_state &= ~(1 << motor_a_bit) & ~(1 << motor_b_bit);
  latch_tx();
}

void AF_DCMotor::run(uint8_t cmd) {
  switch(cmd) {
    case FORWARD:
      latch_state |= (1 << motor_a_bit);    // Set A bit
      latch_state &= ~(1 << motor_b_bit);   // Clear B bit
      break;
    case BACKWARD:
      latch_state &= ~(1 << motor_a_bit);   // Clear A bit
      latch_state |= (1 << motor_b_bit);    // Set B bit
      break;
    case BRAKE:
      latch_state |= (1 << motor_a_bit);    // Set both bits for brake
      latch_state |= (1 << motor_b_bit);
      break;
    case RELEASE:
      latch_state &= ~(1 << motor_a_bit);   // Clear both bits
      latch_state &= ~(1 << motor_b_bit);
      break;
  }
  latch_tx();
}

void AF_DCMotor::setSpeed(uint8_t speed) {
  analogWrite(pwm_pin, speed);
}

// AF_Stepper class implementation
AF_Stepper::AF_Stepper(uint16_t steps, uint8_t num) {
  revsteps = steps;
  steppernum = num;
  currentstep = 0;
  usperstep = 0;
  steppingcounter = 0;
  
  // Initialize motor controller if not done already
  initMotorController();
  
  if (steppernum == 1) {
    // Stepper 1 uses motors M1 and M2
    latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B) & 
                   ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
    latch_tx();
    
    // Enable PWM pins for stepper 1
    pinMode(PWM2A, OUTPUT);  // Pin 11 for M1
    pinMode(PWM2B, OUTPUT);  // Pin 3 for M2
    digitalWrite(PWM2A, HIGH);
    digitalWrite(PWM2B, HIGH);
    
  } else if (steppernum == 2) {
    // Stepper 2 uses motors M3 and M4
    latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B) & 
                   ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
    latch_tx();
    
    // Enable PWM pins for stepper 2
    pinMode(PWM0A, OUTPUT);  // Pin 6 for M3
    pinMode(PWM0B, OUTPUT);  // Pin 5 for M4
    digitalWrite(PWM0A, HIGH);
    digitalWrite(PWM0B, HIGH);
  }
}

void AF_Stepper::setSpeed(uint16_t rpm) {
  usperstep = 60000000L / ((uint32_t)revsteps * (uint32_t)rpm);
  steppingcounter = 0;
}

void AF_Stepper::release(void) {
  if (steppernum == 1) {
    latch_state &= ~(1 << MOTOR1_A) & ~(1 << MOTOR1_B) & 
                   ~(1 << MOTOR2_A) & ~(1 << MOTOR2_B);
  } else if (steppernum == 2) {
    latch_state &= ~(1 << MOTOR3_A) & ~(1 << MOTOR3_B) & 
                   ~(1 << MOTOR4_A) & ~(1 << MOTOR4_B);
  }
  latch_tx();
}

void AF_Stepper::step(uint16_t steps, uint8_t dir, uint8_t style) {
  uint32_t uspers = usperstep;
  uint8_t ret = 0;

  if (style == INTERLEAVE) {
    uspers /= 2;
  } else if (style == MICROSTEP) {
    uspers /= MICROSTEPS;
    steps *= MICROSTEPS;
  }

  while (steps--) {
    ret = onestep(dir, style);
    if (uspers > 16383) {  // Delay longer than 16ms
      delay(uspers / 1000);
      delayMicroseconds(uspers % 1000);
    } else {
      delayMicroseconds(uspers);
    }
    
    steppingcounter += (uspers % 1000);
    if (steppingcounter >= 1000) {
      delay(1);
      steppingcounter -= 1000;
    }
  }
  
  if (style == MICROSTEP) {
    while ((ret != 0) && (ret != MICROSTEPS)) {
      ret = onestep(dir, style);
      if (uspers > 16383) {
        delay(uspers / 1000);
        delayMicroseconds(uspers % 1000);
      } else {
        delayMicroseconds(uspers);
      }
      
      steppingcounter += (uspers % 1000);
      if (steppingcounter >= 1000) {
        delay(1);
        steppingcounter -= 1000;
      }
    }
  }
}

uint8_t AF_Stepper::onestep(uint8_t dir, uint8_t style) {
  uint8_t a, b, c, d;
  uint8_t ocra = 255, ocrb = 255;

  if (steppernum == 1) {
    a = (1 << MOTOR1_A);
    b = (1 << MOTOR2_A);
    c = (1 << MOTOR1_B);
    d = (1 << MOTOR2_B);
  } else if (steppernum == 2) {
    a = (1 << MOTOR3_A);
    b = (1 << MOTOR4_A);
    c = (1 << MOTOR3_B);
    d = (1 << MOTOR4_B);
  } else {
    return 0;
  }

  // Handle different stepping styles
  if (style == SINGLE) {
    if ((currentstep / (MICROSTEPS / 2)) % 2) {
      if (dir == FORWARD) {
        currentstep += MICROSTEPS / 2;
      } else {
        currentstep -= MICROSTEPS / 2;
      }
    } else {
      if (dir == FORWARD) {
        currentstep += MICROSTEPS;
      } else {
        currentstep -= MICROSTEPS;
      }
    }
  } else if (style == DOUBLE) {
    if (!(currentstep / (MICROSTEPS / 2) % 2)) {
      if (dir == FORWARD) {
        currentstep += MICROSTEPS / 2;
      } else {
        currentstep -= MICROSTEPS / 2;
      }
    } else {
      if (dir == FORWARD) {
        currentstep += MICROSTEPS;
      } else {
        currentstep -= MICROSTEPS;
      }
    }
  } else if (style == INTERLEAVE) {
    if (dir == FORWARD) {
      currentstep += MICROSTEPS / 2;
    } else {
      currentstep -= MICROSTEPS / 2;
    }
  }

  if (style == MICROSTEP) {
    if (dir == FORWARD) {
      currentstep++;
    } else {
      currentstep--;
    }

    currentstep += MICROSTEPS * 4;
    currentstep %= MICROSTEPS * 4;

    ocra = ocrb = 0;
    if ((currentstep >= 0) && (currentstep < MICROSTEPS)) {
      ocra = microstepcurve[MICROSTEPS - currentstep];
      ocrb = microstepcurve[currentstep];
    } else if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2)) {
      ocra = microstepcurve[currentstep - MICROSTEPS];
      ocrb = microstepcurve[MICROSTEPS * 2 - currentstep];
    } else if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3)) {
      ocra = microstepcurve[MICROSTEPS * 3 - currentstep];
      ocrb = microstepcurve[currentstep - MICROSTEPS * 2];
    } else if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4)) {
      ocra = microstepcurve[currentstep - MICROSTEPS * 3];
      ocrb = microstepcurve[MICROSTEPS * 4 - currentstep];
    }
  }

  currentstep += MICROSTEPS * 4;
  currentstep %= MICROSTEPS * 4;

  // Set PWM values for microstepping
  if (steppernum == 1) {
    analogWrite(PWM2A, ocra);
    analogWrite(PWM2B, ocrb);
  } else if (steppernum == 2) {
    analogWrite(PWM0A, ocra);
    analogWrite(PWM0B, ocrb);
  }

  // Clear all motor bits
  latch_state &= ~a & ~b & ~c & ~d;

  // Set appropriate motor bits based on step position
  if (style == MICROSTEP) {
    if ((currentstep >= 0) && (currentstep < MICROSTEPS))
      latch_state |= a | b;
    if ((currentstep >= MICROSTEPS) && (currentstep < MICROSTEPS * 2))
      latch_state |= b | c;
    if ((currentstep >= MICROSTEPS * 2) && (currentstep < MICROSTEPS * 3))
      latch_state |= c | d;
    if ((currentstep >= MICROSTEPS * 3) && (currentstep < MICROSTEPS * 4))
      latch_state |= d | a;
  } else {
    switch (currentstep / (MICROSTEPS / 2)) {
      case 0:
        latch_state |= a; // energize coil 1 only
        break;
      case 1:
        latch_state |= a | b; // energize coil 1+2
        break;
      case 2:
        latch_state |= b; // energize coil 2 only
        break;
      case 3:
        latch_state |= b | c; // energize coil 2+3
        break;
      case 4:
        latch_state |= c; // energize coil 3 only
        break;
      case 5:
        latch_state |= c | d; // energize coil 3+4
        break;
      case 6:
        latch_state |= d; // energize coil 4 only
        break;
      case 7:
        latch_state |= d | a; // energize coil 1+4
        break;
    }
  }

  latch_tx();
  return currentstep;
}
