#include <zephyr/kernel.h>
#include "sensor.h"
#include "camera_capture.h"

int main(void) {

    return 0; 
}

K_THREAD_DEFINE(sensor_thread_id, 1024, sensor_thread, NULL, NULL, NULL, 4, 0, 0);
K_THREAD_DEFINE(camera_capture_thread_id, 1024, camera_capture_thread, NULL, NULL, NULL, 4, 0, 0);