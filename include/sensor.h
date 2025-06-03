#ifndef SENSOR_H
#define SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/drivers/gpio.h>

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

void sensor_thread(void *, void *, void *);

ZBUS_CHAN_DECLARE(sensor_chan);

#endif /* SENSOR_H */