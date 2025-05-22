#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

int calc_velocity_kmh(uint32_t time_microseconds, uint32_t distance_millimeters);
void sensor_thread(void *, void *, void *);
int teste(void);

#endif
