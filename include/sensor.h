#ifndef SENSOR_H
#define SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>


enum sensor_event_type {
    SENSOR_EVENT_VEHICLE_DETECTED,
    SENSOR_EVENT_SPEED_CALCULATED
};

struct sensor_event {
    enum sensor_event_type type;
    int speed_kmh;
    bool vehicle_detected;
    bool speed_exceeded;  
};

ZBUS_CHAN_DECLARE(sensor_chan);

void sensor_thread(void *, void *, void *);
int calculate_speed_kmh(double distance_km, double time_h);

#endif /* SENSOR_H */