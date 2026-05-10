#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// SERIAL (Standard ESP32)
#define SERIAL_TX 1
#define SERIAL_RX 3
static const uint8_t TX = SERIAL_TX;
static const uint8_t RX = SERIAL_RX;

// I2C Bus (CyberPi Pins)
#define GROVE_SDA 21
#define GROVE_SCL 22
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// SPI BUS (Display & SD Card)
#define SPI_SCK_PIN 18
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SS_PIN 5

static const uint8_t SS = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t MISO = SPI_MISO_PIN;
static const uint8_t SCK = SPI_SCK_PIN;

// BUTTONS (CyberPi Joystick über AW9523 gemappt)
// In Bruce nutzen wir oft Aliase für den InputHandler
#define BTN_ALIAS "\"OK\""
#define BTN_ACT LOW

// FONT SIZE
#define FP 1
#define FM 2
#define FG 3

// TFT_eSPI Display Setup (ST7789 128x128)
#define HAS_SCREEN 1
#define ROTATION 0
#define MINBRIGHT (uint8_t)1

#define USER_SETUP_LOADED 1
#define ST7789_DRIVER 1
#define TFT_WIDTH 128
#define TFT_HEIGHT 128
#define TFT_RGB_ORDER 0 // 0=RGB, 1=BGR
#define TFT_INVERSION_ON 1

#define TFT_BL 26
#define TFT_RST 33
#define TFT_DC 27
#define TFT_MOSI SPI_MOSI_PIN
#define TFT_SCLK SPI_SCK_PIN
#define TFT_CS SPI_SS_PIN
#define TFT_BACKLIGHT_ON HIGH

#define SMOOTH_FONT 1
#define SPI_FREQUENCY 40000000

// IO EXPANDER (AW9523)
#define USE_IO_EXPANDER
#define IO_EXPANDER_AW9523
#define IO_EXPANDER_ADDR 0x58

// CyberPi Joystick Pins am AW9523
#define IO_EXP_UP 4
#define IO_EXP_DOWN 5
#define IO_EXP_LEFT 6
#define IO_EXP_RIGHT 7
#define IO_EXP_SEL 8

// Deepsleep
#define DEEPSLEEP_WAKEUP_PIN 0 // Button A beim CyberPi
#define DEEPSLEEP_PIN_ACT LOW

#endif /* Pins_Arduino_h */
