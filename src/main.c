#include <zephyr/kernel.h>
#include "sensor.h"

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

void sensor_thread(void *, void *, void *) {

    int example_times_in_microsseconds[2] = {110000, 125000};
    int counter = 0;

    while (1) {

        uint32_t simulation_time = example_times_in_microsseconds[counter];
        
        if (counter == 0) { 
            counter++;
        } else {
            counter = 0; 
        }
        
        int velocity = calc_velocity_kmh(simulation_time, CONFIG_RADAR_SENSOR_DISTANCE_MM);
        bool infracao = detect_infraction(velocity);
        printk("Velocidade: %d | Infração?: %d\n", velocity, infracao);
        k_sleep(K_SECONDS(2));
        // printk("Sensor thread running");
        // k_msleep(1000);
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

void camera_thread(void *, void *, void *) {
    while (1) {
        // printk("Camera thread running");
        k_msleep(1000);
    }
}

K_THREAD_DEFINE(main_thread_id, 1024, main_thread, NULL, NULL, NULL, 2, 0, 0);
K_THREAD_DEFINE(sensor_thread_id, 1024, sensor_thread, NULL, NULL, NULL, 3, 0, 0);
K_THREAD_DEFINE(display_thread_id, 1024, display_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(network_thread_id, 1024, network_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(camera_thread_id, 1024, camera_thread, NULL, NULL, NULL, 6, 0, 0);