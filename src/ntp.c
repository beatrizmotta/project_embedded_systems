#include <zephyr/net/sntp.h>
#include <zephyr/net/socket.h>
#include <zephyr/logging/log.h>
#include <time.h>
#include <string.h>


#define SNTP_SERVER "129.6.15.28"

int get_current_time(struct tm *out_time, int8_t timezone_offset_hours)
{
    struct sntp_ctx ctx;
    struct sntp_time sntp_time;
    struct sockaddr_in addr;
    int err;

    memset(&ctx, 0, sizeof(ctx));
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(123);  // SNTP port

    err = inet_pton(AF_INET, SNTP_SERVER, &addr.sin_addr);
    if (err <= 0) {
        printk("Invalid SNTP server address");
        return -EINVAL;
    }

    
    err = sntp_init(&ctx, (struct sockaddr *)&addr, sizeof(addr));
    if (err < 0) {
        printk("SNTP init failed: %d", err);
        return err;
    }
    
    err = sntp_query(&ctx, 4000, &sntp_time);  
    sntp_close(&ctx);
    
    if (err < 0) {
        printk("SNTP query failed: %d", err);
        return err;
    }
    
    time_t unix_time = sntp_time.seconds + (timezone_offset_hours * 3600);
    if (gmtime_r(&unix_time, out_time) == NULL) {
        printk("Failed to convert to UTC time");
        return -EIO;
    }
    
    

    return 0;
}