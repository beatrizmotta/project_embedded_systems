# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_app)

set(ZEPHYR_EXTRA_KCONFIG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig)
set(ZEPHYR_EXTRA_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/camera_service/")


zephyr_include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/camera_service/include
)
target_sources(app PRIVATE 
    src/main.c 
    src/sensor.c 
    src/ntp.c
    src/http_client.c
    src/http_sender.c
    src/camera_capture.c
    camera_service/src/camera_service.c)

