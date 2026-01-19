# STSPIN220 Stepper Motor Driver Component

This ESP-IDF component provides a driver for the STSPIN220 low-voltage stepper motor driver.

## Features

- Full-step and microstepping support (up to 1/256 step)
- Speed control (RPM)
- Direction control
- Enable/disable motor
- Fault detection and clearing
- Standby mode
- Blocking and non-blocking stepping functions

## Installation

Add this component to your ESP-IDF project:

```bash
idf.py add-dependency "stspin220"
```

Or manually copy the component files to your project's `components/` directory.

## Usage

### Initialization

```c
#include "stspin220.h"

stspin220_t motor = {
    .number_of_steps = 200,  // Steps per revolution for your motor
    .step_pin        = 4,
    .dir_pin         = 5,
    .mode1_pin       = 12,
    .mode2_pin       = 13,
    .en_fault_pin    = 14,
    .stby_reset_pin  = 15
};

void app_main(void)
{
    stspin220_init(&motor);
}
```

### Basic Stepping

```c
// Set microstepping mode
stspin220_set_step_mode(&motor, STSPIN220_STEP_1_16);

// Set speed in RPM
stspin220_set_speed(&motor, 60);

// Step 200 full steps (considering microstepping)
stspin220_step(&motor, 200 * 16);  // For 1/16 microstepping
```

### Blocking Stepping

```c
// Step 100 steps with 1ms delay between each step
stspin220_step_blocking(&motor, 100, 1000);
```

### Motor Control

```c
// Enable/disable motor
stspin220_enable(&motor, true);

// Check if motor is enabled
bool enabled = stspin220_is_enabled(&motor);

// Put motor in standby
stspin220_standby(&motor, true);

// Check for faults
if (stspin220_is_fault(&motor)) {
    stspin220_clear_fault(&motor);
}

// Reset motor
stspin220_reset(&motor);
```

## API Reference

### Types

- `stspin220_step_mode_t`: Enumeration for microstepping modes
- `stspin220_t`: Structure containing motor configuration and state

### Functions

- `void stspin220_init(stspin220_t *dev)`: Initialize the motor driver
- `void stspin220_set_speed(stspin220_t *dev, int32_t rpm)`: Set motor speed in RPM
- `void stspin220_step(stspin220_t *dev, int32_t steps)`: Step the motor (non-blocking)
- `void stspin220_single_step(stspin220_t *dev)`: Perform a single step
- `void stspin220_step_blocking(stspin220_t *dev, int32_t steps, uint32_t delay_us)`: Step the motor with blocking delay
- `bool stspin220_set_step_mode(stspin220_t *dev, stspin220_step_mode_t mode)`: Set microstepping mode
- `int stspin220_microsteps_per_step(stspin220_t *dev)`: Get microsteps per full step
- `void stspin220_enable(stspin220_t *dev, bool state)`: Enable or disable motor
- `bool stspin220_is_enabled(stspin220_t *dev)`: Check if motor is enabled
- `void stspin220_standby(stspin220_t *dev, bool state)`: Put motor in standby mode
- `bool stspin220_is_fault(stspin220_t *dev)`: Check for motor fault
- `void stspin220_clear_fault(stspin220_t *dev)`: Clear motor fault
- `void stspin220_reset(stspin220_t *dev)`: Reset the motor driver

## Microstepping Modes

- `STSPIN220_STEP_FULL`: Full step
- `STSPIN220_STEP_1_2`: 1/2 step
- `STSPIN220_STEP_1_4`: 1/4 step
- `STSPIN220_STEP_1_8`: 1/8 step
- `STSPIN220_STEP_1_16`: 1/16 step
- `STSPIN220_STEP_1_32`: 1/32 step
- `STSPIN220_STEP_1_64`: 1/64 step
- `STSPIN220_STEP_1_128`: 1/128 step
- `STSPIN220_STEP_1_256`: 1/256 step

## Dependencies

- ESP-IDF >= 4.0
- driver
- esp_timer

## License

[Specify your license here]

## Examples

See the `examples/` directory for usage examples.