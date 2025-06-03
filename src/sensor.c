#include "sensor.h"
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <math.h>
#include <zephyr/random/random.h> 


#define MIN_DELAY_MS 50
#define MAX_DELAY_MS 200


#define SENSOR_DISTANCE_MM CONFIG_RADAR_SENSOR_DISTANCE_MM
#define SPEED_LIMIT_KMH CONFIG_RADAR_SPEED_LIMIT_KMH

#define SENSOR1_NODE DT_NODELABEL(sensor1)
#define SENSOR2_NODE DT_NODELABEL(sensor2)

static const struct gpio_dt_spec sensor1 = GPIO_DT_SPEC_GET(SENSOR1_NODE, gpios);
static const struct gpio_dt_spec sensor2 = GPIO_DT_SPEC_GET(SENSOR2_NODE, gpios);

static int64_t first_sensor_time = 0;
static bool vehicle_passed_first = false;

static void sensor1_callback(const struct device *dev, struct gpio_callback *cb,
                           uint32_t pins)
{
    if (!vehicle_passed_first) {
        first_sensor_time = k_uptime_get();
        vehicle_passed_first = true;
        
        struct sensor_event evt = {
            .type = SENSOR_EVENT_VEHICLE_DETECTED,
            .vehicle_detected = true,
            .speed_exceeded = false,
            .speed_kmh = 0
        };
        
        zbus_chan_pub(&sensor_chan, &evt, K_MSEC(100));
    }
}

static void sensor2_callback(const struct device *dev, struct gpio_callback *cb,
                           uint32_t pins)
{
    if (vehicle_passed_first) {
        int64_t second_sensor_time = k_uptime_get();
        int64_t time_diff_ms = second_sensor_time - first_sensor_time;
        
        double distance_km = SENSOR_DISTANCE_MM / 1000000.0;
        double time_h = time_diff_ms / (3600.0 * 1000.0);
        int speed_kmh = (int)(distance_km / time_h);
        
        struct sensor_event evt = {
            .type = SENSOR_EVENT_SPEED_CALCULATED,
            .speed_kmh = speed_kmh,
            .vehicle_detected = true,
            .speed_exceeded = (speed_kmh > SPEED_LIMIT_KMH)
        };
        
        zbus_chan_pub(&sensor_chan, &evt, K_MSEC(100));
        printk("Vehicle speed: %d km/h (%s limit)\n", 
               speed_kmh, evt.speed_exceeded ? "above" : "below");
        
        vehicle_passed_first = false;
    }
}

static struct gpio_callback sensor1_cb_data;
static struct gpio_callback sensor2_cb_data;

void sensor_thread(void *, void *, void *)
{
    if (!device_is_ready(sensor1.port) || !device_is_ready(sensor2.port)) {
        printk("Sensor GPIO devices not ready\n");
        return;
    }
    
    int ret = gpio_pin_configure_dt(&sensor1, GPIO_INPUT);
    ret |= gpio_pin_configure_dt(&sensor2, GPIO_INPUT);
    if (ret != 0) {
        printk("Failed to configure sensor GPIO pins\n");
        return;
    }
    
    ret = gpio_pin_interrupt_configure_dt(&sensor1, GPIO_INT_EDGE_RISING);
    ret |= gpio_pin_interrupt_configure_dt(&sensor2, GPIO_INT_EDGE_RISING);
    if (ret != 0) {
        printk("Failed to configure sensor interrupts\n");
        return;
    }
    
    gpio_init_callback(&sensor1_cb_data, sensor1_callback, BIT(sensor1.pin));
    gpio_init_callback(&sensor2_cb_data, sensor2_callback, BIT(sensor2.pin));
    
    gpio_add_callback(sensor1.port, &sensor1_cb_data);
    gpio_add_callback(sensor2.port, &sensor2_cb_data);
    
    printk("Sensor GPIO initialized\n");
    
    while (1) {
        k_msleep(5000);
        // Simulate sensor 1 trigger
        sensor1_callback(NULL, NULL, 0);

        uint32_t delay_ms = MIN_DELAY_MS + (sys_rand32_get() % (MAX_DELAY_MS - MIN_DELAY_MS + 1));
        printk("Simulated delay: %u ms\n", delay_ms);
        k_msleep(delay_ms); 
        // Simulate sensor 2 trigger
        sensor2_callback(NULL, NULL, 0);
    }
}