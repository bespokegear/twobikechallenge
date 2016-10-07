#include "WaitMode.h"
#include "Buttons.h"
#include "ClockDisplay.h"
#include "GameMode.h"
#include <Arduino.h>

_WaitMode WaitMode;

_WaitMode::_WaitMode()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::WaitMode()"));
#endif
}

void _WaitMode::start()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::start()"));
#endif
    // In case the mode button was pressed while in another mode, clear it.
    ModeButton.wasPressed();
    _modeSelect = false;
}

void _WaitMode::stop()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::stop()"));
#endif
#ifndef NOEEPROM
    // TODO: Check to see if we updated the level, and if so, overwrite with the new value
#endif
}

void _WaitMode::modeUpdate()
{
    if (ModeButton.wasPressed()) {
#ifdef DEBUG
    Serial.println(F("WaitMode mode button pressed."));
#endif
        uint8_t d = GameMode.getDifficulty();
        if (_modeSelect) {
            d = (d%GAME_GOAL_STEPS) + 1;
            GameMode.setDifficulty(d);
        }
        Serial.print(F("Level="));
        Serial.println(d);
        ClockDisplay.display('L', (d/10)%10, d%10);
        _modeSelect = true;
    }
}

