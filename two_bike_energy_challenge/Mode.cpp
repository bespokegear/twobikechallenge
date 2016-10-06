#include "Mode.h"
#include "Config.h"
#include "Util.h"
#include "VinMonitors.h"
#include <Arduino.h>

Mode::Mode() :
    _brownedOut(true)
{
}

void Mode::update()
{
    float vIn = ArduinoVoltage.get();
#if (defined(DEBUGVIN) && defined(DEBUG))
    Serial.print(F("Mode::update, vIn="));
    Serial.println(vIn);
#endif
    if (_brownedOut && vIn > BROWNOUT_HIGH) {
        _exitBrownout();
    }
    else if (!_brownedOut && vIn < BROWNOUT_LOW) {
        _enterBrownout();
    }
    modeUpdate();
}

void Mode::_enterBrownout()
{
#ifdef DEBUG
    Serial.println(F("Mode::_enterBrownout"));
#endif
    _brownedOut = true;
    enterBrownout();
}

void Mode::_exitBrownout()
{
#ifdef DEBUG
    Serial.println(F("Mode::_exitBrownout"));
#endif
    _brownedOut = false;
    exitBrownout();
}

