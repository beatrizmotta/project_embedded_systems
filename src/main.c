#include <zephyr/kernel.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/net_mgmt.h>
#include "sensor.h"
#include "display.h"
#include "ntp.h"

void sensor_thread(void *, void *, void *);
void display_thread(void *, void *, void *);

K_THREAD_DEFINE(sensor_tid, 1024, sensor_thread, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(display_tid, 1024, display_thread, NULL, NULL, NULL, 5, 0, 0);

int main(void) {
    int number = teste();
    printk("Hello from Bia e Fred %d!\n", number);

    // Aguarda a rede estar pronta
    printk("Aguardando rede...\n");
    net_mgmt_event_wait_on_iface(NULL, NET_EVENT_IPV4_ADDR_UP, NULL, NULL, K_FOREVER);

    // Sincroniza data/hora via NTP
    sync_time_via_ntp();

    return 0;
}
