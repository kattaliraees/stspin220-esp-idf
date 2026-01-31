#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stspin220.h"

// Motor 1
#define M1_STEP_PIN GPIO_NUM_11
#define M1_DIR_PIN GPIO_NUM_12
#define M1_MODE1_PIN GPIO_NUM_13
#define M1_MODE2_PIN GPIO_NUM_14
#define M1_STBY_PIN GPIO_NUM_21
#define M1_EN_PIN GPIO_NUM_45

// Motor 2
#define M2_STEP_PIN GPIO_NUM_18
#define M2_DIR_PIN GPIO_NUM_8
#define M2_MODE1_PIN GPIO_NUM_3
#define M2_MODE2_PIN GPIO_NUM_46
#define M2_STBY_PIN GPIO_NUM_9
#define M2_EN_PIN GPIO_NUM_10

stspin220_t motor1 = {
    .number_of_steps = 200,
    .step_pin        = M1_STEP_PIN,
    .dir_pin         = M1_DIR_PIN,
    .mode1_pin       = M1_MODE1_PIN,
    .mode2_pin       = M1_MODE2_PIN,
    .en_fault_pin    = M1_EN_PIN,
    .stby_reset_pin  = M1_STBY_PIN
};

stspin220_t motor2 = {
    .number_of_steps = 200,
    .step_pin        = M2_STEP_PIN,
    .dir_pin         = M2_DIR_PIN,
    .mode1_pin       = M2_MODE1_PIN,
    .mode2_pin       = M2_MODE2_PIN,
    .en_fault_pin    = M2_EN_PIN,
    .stby_reset_pin  = M2_STBY_PIN
};


void app_main(void)
{
    printf("Initializing STSPIN220 stepper motor1 driver...\n");
    stspin220_init(&motor1);

    stspin220_set_step_mode(&motor1, STSPIN220_STEP_1_16);
    stspin220_set_speed(&motor1, 60);  // 60 RPM

    printf("Stepping motor1 200 * 16 steps (one revolution at 1/16 microstep)...\n");
    stspin220_step(&motor1, 200 * 16); // one revolution at 1/16 microstep

    printf("Motor movement complete.\n");

    // Example of blocking step
    printf("Blocking step example: 100 steps forward...\n");
    stspin220_step_blocking(&motor1, 100, 1000); // 100 steps with 1ms delay

    printf("Blocking step complete.\n");

    printf("Initializing STSPIN220 stepper motor2 driver...\n");
    stspin220_init(&motor2);

    stspin220_set_step_mode(&motor2, STSPIN220_STEP_1_16);
    stspin220_set_speed(&motor2, 60);  // 60 RPM

    printf("Stepping motor2 200 * 16 steps (one revolution at 1/16 microstep)...\n");
    stspin220_step(&motor2, 200 * 16); // one revolution at 1/16 microstep

    printf("Motor movement complete.\n");

    // Example of blocking step
    printf("Blocking step example: 100 steps forward...\n");
    stspin220_step_blocking(&motor2, 100, 1000); // 100 steps with 1ms delay

    printf("Blocking step complete.\n");
}
