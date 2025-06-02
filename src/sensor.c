#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

#define MICROSECONDS_ON_A_HOUR 3600000000UL

// static const struct device *gpio_dev;
// static struct gpio_callback cb_sensor1, cb_sensor2;

// static int64_t t1 = 0;
// static int64_t t2 = 0;

int calc_velocity_kmh(uint32_t time_microsseconds, uint32_t distance_millimeters) {
    if (time_microsseconds == 0) return 0; 
    
    uint64_t speed = (uint64_t) distance_millimeters * MICROSECONDS_ON_A_HOUR;
    speed /= (uint64_t) time_microsseconds;
    speed /= 1000000UL; 

    return (int) speed; 
}

bool detect_infraction(int velocity_kmh) {
    return velocity_kmh > CONFIG_RADAR_SPEED_LIMIT_KMH;
}

// int sensors_init(void) {
//     gpio_dev = DEVICE_DT_GET(DT_NODELABEL(gpio0));  // or DT_LABEL(DT_NODELABEL(gpio0))

//     gpio_pin_configure(gpio_dev, 5, GPIO_INPUT | GPIO_INT_EDGE_RISING);
//     gpio_pin_configure(gpio_dev, 6, GPIO_INPUT | GPIO_INT_EDGE_RISING);

//     gpio_pin_interrupt_configure(gpio_dev, 5, GPIO_INT_EDGE_RISING);
//     gpio_pin_interrupt_configure(gpio_dev, 6, GPIO_INT_EDGE_RISING);

//     gpio_init_callback(&cb_sensor1, sensor1_callback, BIT(5));
//     gpio_add_callback(gpio_dev, &cb_sensor1);

//     gpio_init_callback(&cb_sensor2, sensor2_callback, BIT(6));
//     gpio_add_callback(gpio_dev, &cb_sensor2);


// }