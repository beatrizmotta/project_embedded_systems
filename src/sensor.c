#include "sensor.h"
#include <zephyr/kernel.h>
#include <zephyr/random/random.h>
#include <zephyr/logging/log.h>
#include <math.h>


#define SENSOR_DISTANCE_MM CONFIG_RADAR_SENSOR_DISTANCE_MM
#define SPEED_LIMIT_KMH CONFIG_RADAR_SPEED_LIMIT_KMH
#define CAMERA_FAILURE_RATE CONFIG_RADAR_CAMERA_FAILURE_RATE_PERCENT

#define MIN_VEHICLE_INTERVAL_MS 3000  
#define MAX_VEHICLE_INTERVAL_MS 10000 
#define MIN_VEHICLE_SPEED_KMH 30      
#define MAX_VEHICLE_SPEED_KMH 120    

static int64_t first_sensor_time = 0;
static bool vehicle_passed_first = false;

static const char *vehicle_types[] = {
    "car", "motorcycle", "truck", "bus"
};

static void simulate_vehicle_passing(void)
{
    const char *vehicle_type = vehicle_types[sys_rand32_get() % ARRAY_SIZE(vehicle_types)];
    
    int simulated_speed_kmh = MIN_VEHICLE_SPEED_KMH + 
                             (sys_rand32_get() % (MAX_VEHICLE_SPEED_KMH - MIN_VEHICLE_SPEED_KMH));
    
    double distance_km = SENSOR_DISTANCE_MM / 1000000.0; /* Convert mm to km */
    double time_h = distance_km / simulated_speed_kmh;
    int time_between_sensors_ms = (int)(time_h * 3600 * 1000);
    
    printk("Simulating %s approaching at %d km/h\n", vehicle_type, simulated_speed_kmh);
    
    first_sensor_time = k_uptime_get();
    vehicle_passed_first = true;
    
    struct sensor_event first_evt = {
        .type = SENSOR_EVENT_VEHICLE_DETECTED,
        .vehicle_detected = true,
        .speed_exceeded = false,
        .speed_kmh = 0
    };
    zbus_chan_pub(&sensor_chan, &first_evt, K_MSEC(100));
    
    // printk("Vehicle passed first sensor at %lld ms\n", first_sensor_time);
    
    k_msleep(time_between_sensors_ms);
    
    int64_t second_sensor_time = k_uptime_get();
    struct sensor_event second_evt = {
        .type = SENSOR_EVENT_SPEED_CALCULATED,
        .speed_kmh = simulated_speed_kmh,
        .vehicle_detected = true,
        .speed_exceeded = (simulated_speed_kmh > SPEED_LIMIT_KMH)
    };
    
    
    zbus_chan_pub(&sensor_chan, &second_evt, K_MSEC(100));
    
    vehicle_passed_first = false;
}

void sensor_thread(void *, void *, void *)
{
    while (1) {
        uint32_t interval = MIN_VEHICLE_INTERVAL_MS + 
                          (sys_rand32_get() % (MAX_VEHICLE_INTERVAL_MS - MIN_VEHICLE_INTERVAL_MS));
        
        printk("Next vehicle simulation in %d ms\n", interval);
        k_msleep(interval);
        
        simulate_vehicle_passing();
    }
}