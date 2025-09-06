# AFMotor R4 Compatible Library

A drop-in replacement for the Adafruit Motor Shield V1 library that works with Arduino R4 WiFi and other non-AVR boards.

## Overview

This library provides full compatibility with the original AFMotor library API while supporting modern Arduino boards including the Arduino R4 WiFi. It maintains the same function calls and behavior as the original library, making it a true drop-in replacement.

## Installation

### Arduino Library Manager
1. Open the Arduino IDE
2. Go to Tools → Manage Libraries
3. Search for "AFMotor R4 Compatible"
4. Click Install

### Manual Installation
1. Download the library
2. Extract to your Arduino libraries folder
3. Restart the Arduino IDE

## Usage

### DC Motors

```cpp
#include "AFMotor_R4.h"

AF_DCMotor motor(1);  // Create motor on M1

void setup() {
  motor.setSpeed(200);  // Set speed (0-255)
  motor.run(FORWARD);   // Start motor
}

void loop() {
  // Motor control code
}
```

### Stepper Motors

```cpp
#include "AFMotor_R4.h"

AF_Stepper stepper(200, 1);  // 200 steps/rev, use M1&M2

void setup() {
  stepper.setSpeed(10);  // 10 RPM
}

void loop() {
  stepper.step(100, FORWARD, SINGLE);  // 100 steps forward
}
```

## Examples

The library includes several examples:

- **BasicMotorTest** - Simple DC motor control demonstration
- **StepperTest** - Basic stepper motor functionality
- **CompleteExample** - Comprehensive demo of all features

## API Reference

### AF_DCMotor Class

- `AF_DCMotor(uint8_t motor_num)` - Constructor (motor 1-4)
- `void setSpeed(uint8_t speed)` - Set motor speed (0-255)
- `void run(uint8_t direction)` - Control motor (FORWARD, BACKWARD, BRAKE, RELEASE)

### AF_Stepper Class

- `AF_Stepper(uint16_t steps_per_rev, uint8_t motor_num)` - Constructor
- `void setSpeed(uint16_t rpm)` - Set stepper speed in RPM
- `void step(uint16_t steps, uint8_t direction, uint8_t style)` - Move stepper
- `uint8_t onestep(uint8_t direction, uint8_t style)` - Single step
- `void release()` - Release stepper motor

## Technical Notes

This library uses direct port manipulation optimized for the Arduino R4 (Wifi) while maintaining compatibility with other Arduino boards. The original AFMotor library relied on AVR-specific code that doesn't work on ARM-based boards like the R4.

## Contributing

Contributions are welcome! Please submit issues and pull requests on GitHub.

## Credits

Based on the original Adafruit Motor Shield library. Adapted for Arduino R4 compatibility by Mia Müßig and Julian Hein.

Original AFMotor library: https://github.com/adafruit/Adafruit-Motor-Shield-library