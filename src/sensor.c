#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

#define MICROSECONDS_ON_A_HOUR 3600000000UL

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