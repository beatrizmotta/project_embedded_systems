#include <stdint.h>

#define MICROSECONDS_ON_A_HOUR 3600000000

int calc_velocity_kmh(uint32_t time_microsseconds, uint32_t distance_milimeters) {
    if (time_microsseconds == 0) return 0; 
    uint64_t velocity = ((uint64_t) distance_milimeters * MICROSECONDS_ON_A_HOUR) / (time_microsseconds * 1000000); 
    return (int) velocity; 
}

int teste(void) {
    return 1; 
}