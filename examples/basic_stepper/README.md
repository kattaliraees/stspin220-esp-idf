# STSPIN220 Basic Stepper Example

This example demonstrates basic usage of the STSPIN220 stepper motor driver component.

## Hardware Setup

Connect the STSPIN220 to your ESP32 as follows:

- STEP: GPIO 4
- DIR: GPIO 5
- MODE1: GPIO 12
- MODE2: GPIO 13
- EN/FAULT: GPIO 14
- STBY/RESET: GPIO 15

## What it does

- Initializes the stepper motor driver
- Sets microstepping mode to 1/16
- Sets speed to 60 RPM
- Steps the motor for one full revolution (200 steps * 16 microsteps)
- Demonstrates blocking step function

## Building and Flashing

```bash
idf.py build
idf.py flash
idf.py monitor
```