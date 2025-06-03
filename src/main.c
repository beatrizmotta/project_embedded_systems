#include <zephyr/kernel.h>
#include "sensor.h"
#include "camera_capture.h"


int main(void) {
    printk("Hello from Bia e Fred!\n");


    return 0; 
}


void main_thread(void *, void *, void *) {
    while (1) {
        k_msleep(1000);
    }
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