#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "display.h"

void display_thread(void *a, void *b, void *c) {
    while (1) {
        printk("Display thread rodando...\n");
        k_msleep(1000);
    }
}
