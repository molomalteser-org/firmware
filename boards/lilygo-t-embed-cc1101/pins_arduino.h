#ifdef CYBERPI  // <--- Hier beginnt dein CyberPi-Teil
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <stdint.h>

// Serial (Standard CyberPi)
#define SERIAL_TX 1
#define SERIAL_RX 3
static const uint8_t TX = SERIAL_TX;
static const uint8_t RX = SERIAL_RX;

// I2C Bus (CyberPi Pins)
#define GROVE_SDA 21
#define GROVE_SCL 22
static const uint8_t SDA = GROVE_SDA;
static const uint8_t SCL = GROVE_SCL;

// SPI BUS (CyberPi LCD & SD)
#define SPI_SCK_PIN 18
#define SPI_MOSI_PIN 23
#define SPI_MISO_PIN 19
#define SPI_SS_PIN 5
static const uint8_t SS = SPI_SS_PIN;
static const uint8_t MOSI = SPI_MOSI_PIN;
static const uint8_t MISO = SPI_MISO_PIN;
static const uint8_t SCK = SPI_SCK_PIN;

// TFT_eSPI Display Setup (ST7789 128x128)
#define ST7789_DRIVER 1
#define USER_SETUP_LOADED 1
#define SMOOTH_FONT 1
#define TFT_WIDTH 128
#define TFT_HEIGHT 128
#define TFT_INVERSION_ON 1
#define TFT_RGB_ORDER 0 

#define TFT_BL 26
#define TFT_RST 33
#define TFT_DC 27
#define TFT_CS SPI_SS_PIN
#define TFT_MOSI SPI_MOSI_PIN
#define TFT_SCLK SPI_SCK_PIN
#define TFT_BACKLIGHT_ON HIGH

#define HAS_SCREEN
#define ROTATION 0
#define MINBRIGHT (uint8_t)1

// Font Sizes
#define FP 1
#define FM 2
#define FG 3

// IO EXPANDER (AW9523)
#define USE_IO_EXPANDER
#define IO_EXPANDER_AW9523
#define IO_EXPANDER_ADDR 0x58

// CyberPi Joystick Pins am AW9523 gemappt
#define IO_EXP_UP 4
#define IO_EXP_DOWN 5
#define IO_EXP_LEFT 6
#define IO_EXP_RIGHT 7
#define IO_EXP_SEL 8

// Buttons Alias für Bruce-UI
#define HAS_BTN 1
#define BTN_ALIAS "\"Joy\""
#define BTN_ACT LOW

// Deepsleep
#define DEEPSLEEP_WAKEUP_PIN 0
#define DEEPSLEEP_PIN_ACT LOW

#endif /* Pins_Arduino_h */

// --------------------------------------------------------------------
// AB HIER FOLGT DER ORIGINAL T-EMBED CODE (UNVERÄNDERT)
// --------------------------------------------------------------------
#elif defined(T_EMBED_1101)
// ... (hier kommt dein geposteter Code für T_EMBED_1101)
#else 
// ... (hier kommt dein geposteter Code für T-Embed NON CC1101)
#endif
