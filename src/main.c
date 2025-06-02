#include <zephyr/kernel.h>
#include "sensor.h"
#include <zephyr/random/random.h>
#include "camera_service.h"

ZBUS_MSG_SUBSCRIBER_DEFINE(msub_camera_evt);

int main(void) {
    printk("Hello from Bia e Fred!\n");
    return 0; 
}

void main_thread(void *, void *, void *) {
    while (1) {
        // printk("Main thread running");
        k_msleep(1000);
    }
}

int call_camera() {

    int random_key = sys_rand16_get() % 50;

    if (random_key % 2 == 0) {
        return 1; 
    }

    return 0;

}



void sensor_thread(void *, void *, void *) {

    
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

void camera_capture_thread(void *, void *, void *) {
    const struct zbus_channel *chan;
    struct msg_camera_evt evt;

    int err;

    while (1) {

        err = camera_api_capture(K_MSEC(100));
    
        if (err) {
            printk("Error code %d in %s (line:%d)\n", err, __FUNCTION__, __LINE__);
            continue;
        } 
    
        err = zbus_sub_wait_msg(&msub_camera_evt, &chan, &evt, K_FOREVER);
        if (err) {
            printk("Error code %d in %s (line:%d)\n", err, __FUNCTION__, __LINE__);
            continue;
        } 
    
        switch (evt.type) {
            case MSG_CAMERA_EVT_TYPE_DATA:
                const char *plate = evt.captured_data->plate;
                bool plate_validity = is_valid_plate(plate);
                printk("Camera captured data: %s\n", plate);
                break;
            case MSG_CAMERA_EVT_TYPE_ERROR:
                printk("Camera error: %d\n", evt.error_code);
                break;
            default:
                printk("Unknown camera event type: %d\n", evt.type);
                break;
        }
    }

    k_msleep(3000);





}


void display_thread(void *, void *, void *) {
    while (1) {
        // printk("Display thread running");
        k_msleep(1000);
    }
}

void network_thread(void *, void *, void *) {
    while (1) {
        // printk("Network thread running");
        k_msleep(1000);
    }
}



K_THREAD_DEFINE(main_thread_id, 1024, main_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(sensor_thread_id, 1024, sensor_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(camera_capture_thread_id, 1024, camera_capture_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(display_thread_id, 1024, display_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(network_thread_id, 1024, network_thread, NULL, NULL, NULL, 4, 0, 0);