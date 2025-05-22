#include <zephyr/net/socket.h>
#include <zephyr/net/sntp.h>
#include <zephyr/net/net_ip.h>
#include <zephyr/net/hostname.h>
#include <zephyr/sys/printk.h>
#include "ntp.h"

#define NTP_SERVER "pool.ntp.org"
#define NTP_PORT 123
#define TIMEOUT_MS 5000

void sync_time_via_ntp(void) {
    struct sockaddr_in addr;
    struct sntp_time sntp_time;
    int ret;

    struct addrinfo hints = {
        .ai_family = AF_INET,
        .ai_socktype = SOCK_DGRAM
    };
    struct addrinfo *res;

    ret = getaddrinfo(NTP_SERVER, NULL, &hints, &res);
    if (ret != 0 || res == NULL) {
        printk("Erro DNS NTP: %d\n", ret);
        return;
    }

    addr = *(struct sockaddr_in *)res->ai_addr;
    addr.sin_port = htons(NTP_PORT);
    freeaddrinfo(res);

    ret = sntp_request((struct sockaddr *)&addr, sizeof(addr), &sntp_time, TIMEOUT_MS, NULL);
    if (ret < 0) {
        printk("Erro SNTP: %d\n", ret);
        return;
    }

    time_t timestamp = (time_t)sntp_time.seconds;
    struct tm *tm_ptr = gmtime(&timestamp);
    if (tm_ptr) {
        char buf[64];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_ptr);
        printk("Hora atual via NTP: %s UTC\n", buf);
    } else {
        printk("Erro ao converter timestamp.\n");
    }
}
