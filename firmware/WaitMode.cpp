#include "WaitMode.h"
#include "ModeButton.h"
#include "ClockDisplay.h"
#include "GameMode.h"
#include "LED1.h"
#include <Arduino.h>
#include <EEPROM.h>

_WaitMode WaitMode;

_WaitMode::_WaitMode()
{
}

void _WaitMode::start()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::start()"));
#endif
    // In case the mode button was pushed while in another mode, clear it.
    ModeButton.pushed();
    _modeSelect = false;
}

void _WaitMode::stop()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::stop()"));
#endif
    // Check to see if the level in EEPROM is different from the level
    // in GameMode, and save if necessary
    if (GameMode.getLevel() != EEPROM.read(EEPROM_LEVEL_ADDRESS)) {
#ifdef DEBUG
        Serial.print("Saving modified GameMode level to EEPROM");
#endif
#ifndef NOEEPROM
        EEPROM.write(EEPROM_LEVEL_ADDRESS, GameMode.getLevel());
#endif
    }
}

void _WaitMode::modeUpdate()
{
    if (ModeButton.repeat()) {
#ifdef DEBUG
        Serial.println(F("WaitMode mode button pressed."));
#endif
        uint8_t d = GameMode.getLevel();
        uint16_t i;
        if (_modeSelect) {
            d = (d%GAME_LEVEL_MAX) + 1;
            GameMode.setLevel(d);
        }
        Serial.print(F("Level="));
        Serial.println(d);
        ClockDisplay.display('L', (d/10)%10, d%10);
        // Also want to display this on the LEDs
        
        // Here the count down happens in red lights at the base of the LEDs
        for (i=0; i<LED1_COUNT; i++) {
            bool lit = d > i;
            LED1.setPixelColor(i, lit ? LEVEL_COLOUR : P1_OFF_COLOR);
        }
        LED1.show();
        
        
        _modeSelect = true;
    }
}
