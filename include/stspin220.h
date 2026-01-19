#ifndef STSPIN220_H
#define STSPIN220_H

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"

/* Microstepping modes (MODE4 MODE3 MODE2 MODE1 bit order) */
typedef enum {
    STSPIN220_STEP_FULL   = 0b0000,
    STSPIN220_STEP_1_2    = 0b0101,
    STSPIN220_STEP_1_4    = 0b1010,
    STSPIN220_STEP_1_8    = 0b0111,
    STSPIN220_STEP_1_16   = 0b1111,
    STSPIN220_STEP_1_32   = 0b0010,
    STSPIN220_STEP_1_64   = 0b1011,
    STSPIN220_STEP_1_128  = 0b0001,
    STSPIN220_STEP_1_256  = 0b0011
} stspin220_step_mode_t;

typedef struct {
    int16_t number_of_steps;
    int16_t step_pin;
    int16_t dir_pin;
    int16_t mode1_pin;
    int16_t mode2_pin;
    int16_t en_fault_pin;
    int16_t stby_reset_pin;

    uint32_t step_delay_us;
    int32_t step_number;
    uint64_t last_step_time_us;
    stspin220_step_mode_t step_mode;
    bool enabled;
} stspin220_t;

/* API */
void stspin220_init(stspin220_t *dev);

void stspin220_set_speed(stspin220_t *dev, int32_t rpm);
void stspin220_step(stspin220_t *dev, int32_t steps);
void stspin220_single_step(stspin220_t *dev);
void stspin220_step_blocking(stspin220_t *dev, int32_t steps, uint32_t delay_us);

bool stspin220_set_step_mode(stspin220_t *dev, stspin220_step_mode_t mode);
int stspin220_microsteps_per_step(stspin220_t *dev);

void stspin220_enable(stspin220_t *dev, bool state);
bool stspin220_is_enabled(stspin220_t *dev);

void stspin220_standby(stspin220_t *dev, bool state);
bool stspin220_is_fault(stspin220_t *dev);
void stspin220_clear_fault(stspin220_t *dev);
void stspin220_reset(stspin220_t *dev);

#endif

