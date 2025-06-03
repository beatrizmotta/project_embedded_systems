#include "camera_capture.h"
#include "infraction.h" 
#include "http_client.h"
#include <zephyr/kernel.h>

void http_sender_thread(void)
{
    struct infraction_data data;

    while (1) {
        printk("Waiting for infraction data...\n");
        k_msgq_get(&infraction_msgq, &data, K_FOREVER);
        printk("Got infraction data, sending notification...\n");
        int err = send_infraction_notification(data.plate, data.speed_record, data.timestamp, data.image_hash);
    }
}

K_THREAD_DEFINE(http_sender_thread_id, 4096, http_sender_thread, NULL, NULL, NULL, 5, 0, 0);
