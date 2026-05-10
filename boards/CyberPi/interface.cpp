#include "core/powerSave.h"
#include "core/utils.h"
#include <Arduino.h>
#include <interface.h>
#include <globals.h>

// CyberPi nutzt den AW9523 Expander für fast alles
#include <io_expander.h>
extern io_expander io_ext; 

/***************************************************************************************
** Function name: _setup_gpio()
** Description:   Initiales Setup für CyberPi Hardware
***************************************************************************************/
void _setup_gpio() {
    // Initialisiere den I2C Bus für den AW9523
    Wire.begin(I2C_SDA, I2C_SCL);
    
    if (!io_ext.init(IO_EXPANDER_ADDR, &Wire)) {
        Serial.println("CyberPi AW9523 nicht gefunden!");
    } else {
        Serial.println("CyberPi I/O Expander gestartet.");
        // Backlight Pin am Expander als Output definieren
        io_ext.setPinDirection(1, OUTPUT); 
    }

    bruceConfig.colorInverted = 0;
}

/***************************************************************************************
** Function name: _post_setup_gpio()
** Description:   Aktiviert das Display-Backlight
***************************************************************************************/
void _post_setup_gpio() {
    // CyberPi Backlight einschalten (Pin 1 am AW9523)
    io_ext.turnPinOnOff(1, HIGH);
    Serial.println("Backlight aktiv.");
}

/*********************************************************************
** Function: _setBrightness
** Description: Steuert die Helligkeit über den Expander
**********************************************************************/
void _setBrightness(uint8_t brightval) {
    // Da der AW9523 auch analogWrite (PWM) kann, nutzen wir das für das Backlight
    // brightval kommt in 0-100%, wir rechnen um auf 0-255
    uint8_t pwm_val = (brightval * 255) / 100;
    
    // Pin 1 ist am CyberPi oft das Backlight
    io_ext.analogWrite(1, pwm_val);
}

/*********************************************************************
** Function: InputHandler
** Description: Liest den CyberPi Joystick über den AW9523 aus
**********************************************************************/
void InputHandler(void) {
    static long d_tmp = 0;
    if (millis() - d_tmp > 150) { // Debounce / Verzögerung
        
        // Wir lesen die Pins vom Expander (definiert in der platformio.ini)
        bool up    = !io_ext.readPin(IO_EXP_UP);    // Low-Aktiv
        bool down  = !io_ext.readPin(IO_EXP_DOWN);
        bool left  = !io_ext.readPin(IO_EXP_LEFT);
        bool right = !io_ext.readPin(IO_EXP_RIGHT);
        bool sel   = !io_ext.readPin(IO_EXP_SEL);

        if (up || down || left || right || sel) {
            if (wakeUpScreen()) {
                d_tmp = millis();
                return; 
            }
            
            AnyKeyPress = true;
            PrevPress = up;
            NextPress = down;
            EscPress  = left;
            SelPress  = sel;
            // 'right' könnte für weitere Funktionen wie Page-Switch genutzt werden
            
            d_tmp = millis();
        }
    }
}

/*********************************************************************
** Function: powerOff
** Description: Tiefschlaf für den CyberPi
**********************************************************************/
void powerOff() {
    // Schalte Display-Licht aus bevor wir schlafen
    io_ext.turnPinOnOff(1, LOW);
    esp_deep_sleep_start();
}

void checkReboot() {}
