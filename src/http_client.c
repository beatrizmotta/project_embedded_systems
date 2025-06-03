#include <zephyr/net/socket.h>
#include <zephyr/net/http/client.h>
#include <zephyr/net/net_ip.h>
#include <stdio.h>
#include <string.h>
#include "http_client.h"

#define SERVER_HOST CONFIG_SERVER_HOST  
#define SERVER_PORT CONFIG_SERVER_PORT
#define SERVER_URL CONFIG_SERVER_URL

int send_infraction_notification(const char *plate, int speed, const char *timestamp, const char *image_hash) {
    int sock;
    struct sockaddr_in server_addr;
    char request_buf[1024];
    char response_buf[512];
    char json_payload[512];

    snprintf(json_payload, sizeof(json_payload),
        "{ \"plate\": \"%s\", \"timestamp\": \"%s\", \"recorded_speed\": %d, \"image_hash\": \"%s\" }",
        plate, timestamp, speed, image_hash);

    snprintf(request_buf, sizeof(request_buf),
        "POST %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        SERVER_URL, SERVER_HOST, SERVER_PORT,
        strlen(json_payload), json_payload);

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printk("Failed to create socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_HOST, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printk("Failed to connect to server\n");
        close(sock);
        return -1;
    }

    send(sock, request_buf, strlen(request_buf), 0);
    int received = recv(sock, response_buf, sizeof(response_buf) - 1, 0);
    if (received > 0) {
        response_buf[received] = '\0';
        printk("--------------------\n");
        printk("Server response from notification:\n%s\n", response_buf);
        printk("--------------------\n");
    } else {
        printk("No response received from server\n");
    }

    close(sock);

    return 0;
}
