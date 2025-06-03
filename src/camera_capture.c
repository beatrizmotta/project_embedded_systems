#include "ntp.h"
#include <time.h>
#include <zephyr/random/random.h>
#include "camera_service.h"
#include "sensor.h"
#include <zephyr/zbus/zbus.h>

ZBUS_MSG_SUBSCRIBER_DEFINE(msub_camera_evt);
ZBUS_MSG_SUBSCRIBER_DEFINE(msub_sensor_evt); 

ZBUS_CHAN_DEFINE(sensor_chan,             
         struct sensor_event,    
         NULL,                          
         NULL,                           
         ZBUS_OBSERVERS(msub_sensor_evt),           
         ZBUS_MSG_INIT(0)                
);

ZBUS_SUBSCRIBER_DEFINE(sensor_consumer, 4);


int should_call_camera() {
    if (CONFIG_RADAR_CAMERA_FAILURE_RATE_PERCENT > 0) {
        if ((sys_rand32_get() % 100) < CONFIG_RADAR_CAMERA_FAILURE_RATE_PERCENT) {
            printk("Camera failed to trigger (simulated)\n");
            return 0;
        }
    }
    return 1; 
}


void camera_capture_thread(void *, void *, void *) {
    const struct zbus_channel *chan;
    struct msg_camera_evt cam_evt;
    struct sensor_event sensor_evt;
    struct tm current_time;
    int err;

    while (1) {
        // First check for sensor events
        err = zbus_sub_wait_msg(&msub_sensor_evt, &chan, &sensor_evt, K_MSEC(100));

        if (!err && chan == &sensor_chan) {
            switch (sensor_evt.type) {
                case SENSOR_EVENT_VEHICLE_DETECTED:
                    printk("Vehicle detected at first sensor\n");
                    break;
                    
                case SENSOR_EVENT_SPEED_CALCULATED:
                    printk("Speed calculated: %d km/h (%s limit)\n", 
                           sensor_evt.speed_kmh,
                           sensor_evt.speed_exceeded ? "ABOVE" : "below");
                    
                    if (sensor_evt.speed_exceeded && should_call_camera()) {
                        printk("Triggering camera for speeding vehicle\n");
                        camera_api_capture(K_MSEC(100));
                    }
                    break;
            }
            continue;  
        }

        err = zbus_sub_wait_msg(&msub_camera_evt, &chan, &cam_evt, K_MSEC(100));
        if (!err) {
            switch (cam_evt.type) {
                case MSG_CAMERA_EVT_TYPE_DATA:
                    if (is_valid_plate(cam_evt.captured_data->plate)) {
                        get_current_time(&current_time, 0);
                        printk("Valid plate captured: %s at %02d:%02d:%02d\n",
                               cam_evt.captured_data->plate,
                               current_time.tm_hour,
                               current_time.tm_min,
                               current_time.tm_sec);
                    } else {
                        printk("Invalid plate captured: %s at %02d:%02d:%02d\n", 
                            cam_evt.captured_data->plate,
                            current_time.tm_hour,
                            current_time.tm_min,
                            current_time.tm_sec);
                    }
                    break;
                    
                case MSG_CAMERA_EVT_TYPE_ERROR:
                    printk("Camera error: %d\n", cam_evt.error_code);
                    break;
                    
                default:
                    printk("Unknown camera event type: %d\n", cam_evt.type);
                    break;
            }
        }
    }
}

int is_valid_plate(const char *plate) {
    int letter_counter = 0; 
    int number_counter = 0; 
    for (int i = 0; plate[i] != '\0'; i++) {

        if (plate[i] == ' ') {
            continue; 
        } 

        if (plate[i] >= 48 && plate[i] <= 57) {
            number_counter++;
            continue;
        } 
        
        if (plate[i] >= 65 && plate[i] <= 90) {
            letter_counter++;
            continue;
        }
    }

    if (letter_counter == 4 && number_counter == 3) {
        return 1; 
    } 

    return 0; 

}


