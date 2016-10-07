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
}

void _WaitMode::modeUpdate()
{
    if (ModeButton.wasPressed()) {
#ifdef DEBUG
    Serial.println(F("WaitMode mode button pressed."));
#endif
        _GameMode::Difficulty d = GameMode.getDifficulty();
        if (_modeSelect) {
            switch (d) {
            case _GameMode::Easy:
                d = _GameMode::Medium;
                break;
            case _GameMode::Medium:
                d = _GameMode::Hard;
                break;
            case _GameMode::Hard:
                d = _GameMode::Easy;
                break;
            }
            GameMode.setDifficulty(d);
        }
        switch (d) {
        case _GameMode::Easy:
            ClockDisplay.display(" L1");
            break;
        case _GameMode::Medium:
            ClockDisplay.display(" L2");
            break;
        case _GameMode::Hard:
            ClockDisplay.display(" L3");
            break;
        }
        _modeSelect = true;
    }
}

