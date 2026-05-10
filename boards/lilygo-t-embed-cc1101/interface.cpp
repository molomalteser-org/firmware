#include "core/powerSave.h"
#include <globals.h>
#include <interface.h>
#include <Wire.h>
#include "core/i2c_finder.h"
#include "modules/rf/rf_utils.h"

// CyberPi spezifisch: AW9523 Expander
#include <io_expander.h>
extern io_expander io_ext; 

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   Initiales Setup für CyberPi (I2C & Expander)
***************************************************************************************/
void _setup_gpio() {
    // I2C für CyberPi starten
    Wire.begin(GROVE_SDA, GROVE_SCL);

    // Initialisiere den Expander für Joystick und Backlight
    if (!io_ext.init(IO_EXPANDER_ADDR, &Wire)) {
        Serial.println("CyberPi AW9523 nicht gefunden!");
    } else {
        // Pin 1 am AW9523 ist das Backlight
        io_ext.setPinDirection(1, OUTPUT); 
    }

    // SPI CS Pins sicherheitshalber auf HIGH setzen
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    bruceConfig.colorInverted = 0;
}

/***************************************************************************************
** Function name: getBattery() / isCharging()
***************************************************************************************/
int getBattery() { return 100; } // CyberPi hat keinen Gauge-Chip
bool isCharging() { return true; }

/*********************************************************************
** Function: _setBrightness
** Description: Steuert Helligkeit über AW9523 PWM
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        io_ext.turnPinOnOff(1, LOW);
    } else {
        uint8_t pwm_val = (brightval * 255) / 100;
        io_ext.analogWrite(1, pwm_val);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles: CyberPi Joystick via AW9523
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = millis();
    if (millis() - tm < 200 && !LongPress) return;

    // Joystick-Pins vom Expander lesen (Low-Aktiv)
    bool _u = !io_ext.readPin(IO_EXP_UP);
    bool _d = !io_ext.readPin(IO_EXP_DOWN);
    bool _l = !io_ext.readPin(IO_EXP_LEFT);
    bool _r = !io_ext.readPin(IO_EXP_RIGHT);
    bool _s = !io_ext.readPin(IO_EXP_SEL);

    if (_u || _d || _l || _r || _s) {
        tm = millis();
        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;

        if (_l) PrevPress = true;
        if (_r) NextPress = true;
        if (_u) { UpPress = true; PrevPagePress = true; }
        if (_d) { DownPress = true; NextPagePress = true; }
        if (_s) SelPress = true;
    }
}

void powerOff() {
    io_ext.turnPinOnOff(1, LOW);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** Description: Joystick Links halten für Power-Menü
**********************************************************************/
void checkReboot() {
    if (io_ext.readPin(IO_EXP_LEFT) == LOW) {
        uint32_t time_count = millis();
        while (io_ext.readPin(IO_EXP_LEFT) == LOW) {
            if (millis() - time_count > 500) {
                int countDown = (millis() - time_count) / 1000 + 1;
                if (countDown < 3) {
                    tft.drawCentreString("PWR OFF IN " + String(countDown) + "/2", tftWidth / 2, 12, 1);
                } else {
                    powerOff();
                }
                delay(10);
            }
        }
        tft.fillRect(0, 12, tftWidth, 18, bruceConfig.bgColor);
        drawStatusBar();
    }
}
