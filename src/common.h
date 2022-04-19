#pragma once

#include <Arduino.h>

#include <SPI.h>
#include <string.h>


using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;


//  Pinnacle TM0XX0XX with Arduino
//  Hardware Interface
//  GND
//  +3.3V
//  SCK = Pin 13
//  MISO = Pin 12
//  MOSI = Pin 11
//  SS = Pin 10 - or - Pin 8
//  DR = Pin 9  - or - Pin 7

// Hardware pin-number labels
// SPI port definitions
#define SCK_PIN   13
#define DIN_PIN   12
#define DOUT_PIN  11
#define CS0_PIN   10    // Chip Select for Sensor 0
#define CS1_PIN   8     // Chip Select for Sensor 1

// Pinnacle Data-Ready pins
#define DR0_PIN   9     // Data-Ready for Sensor 0
#define DR1_PIN   7     // Data-Ready for Sensor 1

// I2C pins (not used in this demo)
#define SDA_PIN   18
#define SCL_PIN   19

#define LED_0     21
#define LED_1     20

// Masks for Cirque Register Access Protocol (RAP)
#define WRITE_MASK  0x80
#define READ_MASK   0xA0

// Register config values for this demo
#define SYSCONFIG_1   0x00
#define FEEDCONFIG_1  0x02
#define FEEDCONFIG_2  0x1F
#define Z_IDLE_COUNT  0x05

// Coordinate scaling values
#define PINNACLE_XMAX     2047    // max value Pinnacle can report for X (0 to (8 * 256) - 1)
#define PINNACLE_YMAX     1535    // max value Pinnacle can report for Y (0 to (6 * 256) - 1)
#define PINNACLE_X_LOWER  127     // min "reachable" X value
#define PINNACLE_X_UPPER  1919    // max "reachable" X value
#define PINNACLE_Y_LOWER  63      // min "reachable" Y value
#define PINNACLE_Y_UPPER  1471    // max "reachable" Y value
#define PINNACLE_X_RANGE  (PINNACLE_X_UPPER-PINNACLE_X_LOWER)
#define PINNACLE_Y_RANGE  (PINNACLE_Y_UPPER-PINNACLE_Y_LOWER)
#define ZONESCALE 256   // divisor for reducing x,y values to an array index for the LUT
#define ROWS_Y ((PINNACLE_YMAX + 1) / ZONESCALE)
#define COLS_X ((PINNACLE_XMAX + 1) / ZONESCALE)

// ADC-attenuation settings (held in BIT_7 and BIT_6)
// 1X = most sensitive, 4X = least sensitive
#define ADC_ATTENUATE_1X   0x00
#define ADC_ATTENUATE_2X   0x40
#define ADC_ATTENUATE_3X   0x80
#define ADC_ATTENUATE_4X   0xC0

#define ADC_LOWER_DEFAULT  0x02

// Select sensors that are active
// 1 = On, 0 = Off
#define SENSE0_SELECT         1
#define SENSE1_SELECT         1
// Select the overlay type
// 1 = Curved Overlay, 0 = Flat Overlay
#define SENSE0_OVERLAY_CURVE  1
#define SENSE1_OVERLAY_CURVE  1

#define DEFAULT_WRITE_DELAY 50
