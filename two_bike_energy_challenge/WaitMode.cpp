#include "WaitMode.h"
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
}

void _WaitMode::stop()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::stop()"));
#endif
}

void _WaitMode::modeUpdate()
{
}

