#include "stspin220.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void cfg_out(int pin, int level)
{
    if (pin < 0) return;
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << pin,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = 0,
        .pull_up_en = 0,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
    gpio_set_level(pin, level);
}

static void cfg_in_pullup(int pin)
{
    if (pin < 0) return;
    gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << pin,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&cfg);
}

void stspin220_init(stspin220_t *dev)
{
    cfg_out(dev->step_pin, 1);
    cfg_out(dev->dir_pin, 1);

    if (dev->mode1_pin >= 0) cfg_out(dev->mode1_pin, 1);
    if (dev->mode2_pin >= 0) cfg_out(dev->mode2_pin, 1);

    if (dev->en_fault_pin >= 0) cfg_in_pullup(dev->en_fault_pin);
    if (dev->stby_reset_pin >= 0) cfg_out(dev->stby_reset_pin, 1);

    dev->step_delay_us = 1000;
    dev->step_number = 0;
    dev->last_step_time_us = 0;
    dev->step_mode = STSPIN220_STEP_1_16;
    dev->enabled = true;
}

int stspin220_microsteps_per_step(stspin220_t *dev)
{
    switch (dev->step_mode)
    {
        case STSPIN220_STEP_FULL: return 1;
        case STSPIN220_STEP_1_2: return 2;
        case STSPIN220_STEP_1_4: return 4;
        case STSPIN220_STEP_1_8: return 8;
        case STSPIN220_STEP_1_16: return 16;
        case STSPIN220_STEP_1_32: return 32;
        case STSPIN220_STEP_1_64: return 64;
        case STSPIN220_STEP_1_128: return 128;
        case STSPIN220_STEP_1_256: return 256;
        default: return 16;
    }
}

void stspin220_set_speed(stspin220_t *dev, int32_t rpm)
{
    if (rpm <= 0) {
        dev->step_delay_us = 1000000;
        return;
    }

    int micro = stspin220_microsteps_per_step(dev);
    dev->step_delay_us =
        (60LL * 1000LL * 1000LL) /
        (dev->number_of_steps * micro * rpm);

    if (dev->step_delay_us < 1)
        dev->step_delay_us = 1;
}

void stspin220_single_step(stspin220_t *dev)
{
    gpio_set_level(dev->step_pin, 0);
    esp_rom_delay_us(1);
    gpio_set_level(dev->step_pin, 1);
}

void stspin220_step(stspin220_t *dev, int32_t steps)
{
    int32_t steps_left = (steps > 0) ? steps : -steps;

    gpio_set_level(dev->dir_pin, steps > 0);
    esp_rom_delay_us(1);

    while (steps_left > 0)
    {
        uint64_t now = esp_timer_get_time();

        if ((now - dev->last_step_time_us) >= dev->step_delay_us)
        {
            stspin220_single_step(dev);

            if (steps > 0)
            {
                dev->step_number++;
                if (dev->step_number == dev->number_of_steps)
                    dev->step_number = 0;
            }
            else
            {
                if (dev->step_number == 0)
                    dev->step_number = dev->number_of_steps;
                dev->step_number--;
            }

            steps_left--;
            dev->last_step_time_us = now;
        }
    }
}

void stspin220_step_blocking(stspin220_t *dev, int32_t steps, uint32_t delay_us)
{
    int32_t count = (steps > 0) ? steps : -steps;

    gpio_set_level(dev->dir_pin, steps > 0);
    esp_rom_delay_us(1);

    while (count--)
    {
        stspin220_single_step(dev);
        esp_rom_delay_us(delay_us);
    }
}

bool stspin220_set_step_mode(stspin220_t *dev, stspin220_step_mode_t mode)
{
    if (dev->stby_reset_pin < 0)
        return false;

    uint8_t bits = (uint8_t)mode;

    gpio_set_level(dev->stby_reset_pin, 0);
    vTaskDelay(pdMS_TO_TICKS(1));

    if (dev->mode1_pin >= 0) gpio_set_level(dev->mode1_pin, bits & 0x01);
    if (dev->mode2_pin >= 0) gpio_set_level(dev->mode2_pin, bits & 0x02);
    gpio_set_level(dev->step_pin, bits & 0x04);
    gpio_set_level(dev->dir_pin,  bits & 0x08);

    gpio_set_level(dev->stby_reset_pin, 1);

    dev->step_mode = mode;
    return true;
}

void stspin220_enable(stspin220_t *dev, bool state)
{
    if (dev->en_fault_pin < 0)
        return;

    if (state) {
        cfg_in_pullup(dev->en_fault_pin);
    } else {
        cfg_out(dev->en_fault_pin, 0);
    }

    dev->enabled = state;
}

bool stspin220_is_enabled(stspin220_t *dev)
{
    if (dev->en_fault_pin < 0)
        return true;
    return dev->enabled;
}

bool stspin220_is_fault(stspin220_t *dev)
{
    if (dev->en_fault_pin < 0)
        return false;

    return (gpio_get_level(dev->en_fault_pin) == 0);
}

void stspin220_clear_fault(stspin220_t *dev)
{
    if (dev->en_fault_pin < 0)
        return;

    cfg_out(dev->en_fault_pin, 0);
    esp_rom_delay_us(1000);
    cfg_in_pullup(dev->en_fault_pin);

    dev->enabled = true;
}

void stspin220_standby(stspin220_t *dev, bool state)
{
    if (dev->stby_reset_pin < 0) return;

    if (state) {
        gpio_set_level(dev->stby_reset_pin, 0);
    } else {
        stspin220_set_step_mode(dev, dev->step_mode);
    }
}

void stspin220_reset(stspin220_t *dev)
{
    if (dev->stby_reset_pin < 0) return;

    stspin220_standby(dev, true);
    vTaskDelay(pdMS_TO_TICKS(1));
    stspin220_standby(dev, false);
}

/*
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
    stspin220_init(&motor);

    stspin220_set_step_mode(&motor, STSPIN220_STEP_1_16);
    stspin220_set_speed(&motor, 60);  // 60 RPM

    stspin220_step(&motor, 200 * 16); // one revolution at 1/16 microstep
}

*/
