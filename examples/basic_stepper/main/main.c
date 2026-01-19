#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stspin220.h"

stspin220_t motor = {
    .number_of_steps = 200,
    .step_pin        = 4,
    .dir_pin         = 5,
    .mode1_pin       = 12,
    .mode2_pin       = 13,
    .en_fault_pin    = 14,
    .stby_reset_pin  = 15
};

void app_main(void)
{
    printf("Initializing STSPIN220 stepper motor driver...\n");
    stspin220_init(&motor);

    stspin220_set_step_mode(&motor, STSPIN220_STEP_1_16);
    stspin220_set_speed(&motor, 60);  // 60 RPM

    printf("Stepping motor 200 * 16 steps (one revolution at 1/16 microstep)...\n");
    stspin220_step(&motor, 200 * 16); // one revolution at 1/16 microstep

    printf("Motor movement complete.\n");

    // Example of blocking step
    printf("Blocking step example: 100 steps forward...\n");
    stspin220_step_blocking(&motor, 100, 1000); // 100 steps with 1ms delay

    printf("Blocking step complete.\n");
}