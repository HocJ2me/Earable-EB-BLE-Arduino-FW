/*
 * File: configs.h
 * Authors: Hank <lctuyenvn@gmail.com>
 * Create: 2019-04-17 16:51:03
 *
 * Copyright (c) 2019 EmBCI. All right reserved.
 *
 * Modify this file to quickly configure ESP32_Sandbox,
 * which is the firmware of ESP32 used in project EmBCI.
 */

#ifndef CONFIGS_H
#define CONFIGS_H



// Earable EB Define
#define DeviceName  "ESP32-DEVICE"
#define DEVICE_HARDWARE_VERSION "EBMEO_ESP32_DEVICE_0_1"
#define DEVICE_BATTERY_LEVEL "ddd"
#define DEVICE_NAME DeviceName
#define DEVICE_ID "EB12345678"
#define DEVICE_SERIAL_NUMBERR "EB123456789ABCDEF"
#define DEVICE_UNIX_TIME "1662972517"


// SPI master parameters (connected with ADS1299)
#define GPIO_MOSI        23
#define GPIO_MISO        19
#define GPIO_SCLK        18
#define GPIO_CS          15
#define GPIO_DRDY        27

// SPI slave parameters (send out data through SPI)
#define PACKETSIZE       256
#define SLAVESENDTIMEOUT 1000
#define PIN_CS           GPIO_CS
#define PIN_DRDY         GPIO_DRDY

// FIFO data queue
#define M_BUFFERSIZE     8192
#define M_BUFFERSIZESPI  256
#define bufftype int32_t

// Blink test pin
#define PIN_BLINK        LED_BUILTIN

// GPIO number that can wakeup ESP32 from sleep mode
#define PIN_WAKEUP       0

// UART number used for console, choose one from:
//  UART_NUM_0, UART_NUM_1, UART_NUM_2
#define UART_CONSOLE     UART_NUM_0

// Baudrate of console UART, default 115200
#define UART_BAUDRATE    9600

// Clock frequence in Hz of SPI master (connected with ADS1299)
#define CLK_SPI          1000000


// Log level, choose one from:
//  5 ESP_LOG_VERBOSE
//  4 ESP_LOG_DEBUG
//  3 ESP_LOG_INFO
//  2 ESP_LOG_WARN
//  1 ESP_LOG_ERROR
//  0 ESP_LOG_NONE
#define LOG_LEVEL        ESP_LOG_DEBUG

// Whether enable WiFi echo (direct UART/I2C message to WiFi)
#define WIFI_ECHO        false


#endif // CONFIGS_H
