#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>
#include <interface.h>
#include <globals.h>
#include <Wire.h>

// CyberPi nutzt den AW9523 Expander für Joystick und Backlight
#include "HAL/io_expander/io_expander.h"
extern io_expander io_ext; 

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   Initiales Setup der CyberPi Hardware
***************************************************************************************/
void _setup_gpio() {
    // I2C für den AW9523 starten
    Wire.begin(GROVE_SDA, GROVE_SCL);

    // Initialisiere den Expander
    if (!io_ext.init(IO_EXPANDER_ADDR, &Wire)) {
        Serial.println("CyberPi AW9523 nicht gefunden!");
    } else {
        // Backlight Pin am Expander als Ausgang (CyberPi Pin 1)
        io_ext.setPinDirection(1, OUTPUT); 
    }

    // SPI CS Pins für interne Module auf HIGH (deaktiviert) setzen
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(SDCARD_CS, OUTPUT);
    digitalWrite(SDCARD_CS, HIGH);

    bruceConfig.colorInverted = 0;
}

/***************************************************************************************
** Function name: getBattery()
** Description:   CyberPi hat meist keinen Gauge-Chip, daher Dummy-Wert oder ADC
***************************************************************************************/
int getBattery() {
    return 100; // CyberPi hängt meist am USB, daher "voll"
}

bool isCharging() {
    return true; // Per Default true, da USB-betrieben
}

/*********************************************************************
** Function: _setBrightness
** Description: Steuert das Licht über die PWM-Funktion des AW9523
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    if (brightval == 0) {
        io_ext.turnPinOnOff(1, LOW);
    } else {
        // Umrechnung 0-100% in 0-255 PWM
        uint8_t pwm_val = (brightval * 255) / 100;
        io_ext.analogWrite(1, pwm_val);
    }
}

/*********************************************************************
** Function: InputHandler
** Handles: Up, Down, Left, Right, Select via AW9523 Expander
**********************************************************************/
void InputHandler(void) {
    static unsigned long tm = 0;
    if (millis() - tm < 200 && !LongPress) return;

    // Pins vom AW9523 lesen (die Pins sind in deiner pins_arduino.h definiert)
    bool _u = io_ext.readPin(IO_EXP_UP);    // Joystick hoch
    bool _d = io_ext.readPin(IO_EXP_DOWN);  // Joystick runter
    bool _l = io_ext.readPin(IO_EXP_LEFT);  // Joystick links
    bool _r = io_ext.readPin(IO_EXP_RIGHT); // Joystick rechts
    bool _s = io_ext.readPin(IO_EXP_SEL);   // Joystick Druck (Select)

    // Logik: AW9523 Pins sind meist Pull-Up (LOW wenn gedrückt)
    if (!_u || !_d || !_l || !_r || !_s) {
        tm = millis();
        if (!wakeUpScreen()) AnyKeyPress = true;
        else return;

        if (!_l) PrevPress = true;
        if (!_r) NextPress = true;
        if (!_u) { UpPress = true; PrevPagePress = true; }
        if (!_d) { DownPress = true; NextPagePress = true; }
        if (!_s) SelPress = true;
    }
}

/*********************************************************************
** Function: powerOff
** Description: Schaltet das Licht aus und geht in den DeepSleep
**********************************************************************/
void powerOff() {
    io_ext.turnPinOnOff(1, LOW);
    esp_deep_sleep_start();
}

/*********************************************************************
** Function: checkReboot
** Description: Hält man "Links" gedrückt, wird ein Power-Off simuliert
**********************************************************************/
void checkReboot() {
    if (io_ext.readPin(IO_EXP_LEFT) == LOW) { // Joystick links als "ESC" Ersatz
        uint32_t time_count = millis();
        while (io_ext.readPin(IO_EXP_LEFT) == LOW) {
            if (millis() - time_count > 500) {
                int countDown = (millis() - time_count) / 1000 + 1;
                if (countDown < 3) {
                    tft.drawCentreString("OFF IN " + String(countDown) + "/2", tftWidth / 2, 12, 1);
                } else {
                    powerOff();
                }
                delay(10);
            }
        }
        tft.fillRect(0, 12, tftWidth, 10, bruceConfig.bgColor);
    }
}
