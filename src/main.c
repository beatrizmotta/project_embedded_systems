#include <zephyr/kernel.h>
#include "sensor.h"

int main(void) {
    int number = teste();
    printk("Hello from Bia e Fred %d!\n", number);
    return 0; 
}

void sensor_thread(void *, void *, void *) {
    while (1) {
        k_msleep(1000);
    }
}

void display_thread(void *, void *, void *) {
    while (1) {
        k_msleep(1000);
    }
}