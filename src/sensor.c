#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <stdint.h>
#include "sensor.h"

#define MICROSECONDS_IN_HOUR 3600000000ULL

#define SENSOR1_PIN 5
#define SENSOR2_PIN 6
#define SENSOR_PORT DT_LABEL(DT_NODELABEL(gpioa)) // Porta A no QEMU

static const struct gpio_dt_spec sensor1 = GPIO_DT_SPEC_GET(DT_NODELABEL(gpioa), gpios);
static const struct gpio_dt_spec sensor2 = GPIO_DT_SPEC_GET(DT_NODELABEL(gpioa), gpios);

int calc_velocity_kmh(uint32_t time_microseconds, uint32_t distance_millimeters) {
    if (time_microseconds == 0) return 0;
    uint64_t velocity = ((uint64_t)distance_millimeters * MICROSECONDS_IN_HOUR) / ((uint64_t)time_microseconds * 1000000);
    return (int)velocity;
}

int teste(void) {
    return 1;
}

void sensor_thread(void *a, void *b, void *c) {
    printk("Thread dos sensores iniciada.\n");

    while (1) {
        uint32_t simulated_time_us = 800000;
        uint32_t distance_mm = 1000;

        int velocity = calc_velocity_kmh(simulated_time_us, distance_mm);
        printk("Velocidade simulada: %d km/h\n", velocity);

        k_msleep(2000);
    }
}
