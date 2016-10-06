#include "WaitMode.h"
#include <Arduino.h>

WaitMode::WaitMode()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::WaitMode()"));
#endif
}

void WaitMode::start()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::start()"));
#endif
}

void WaitMode::stop()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::stop()"));
#endif
}

void WaitMode::modeUpdate()
{
#ifdef DEBUG
    Serial.println(F("WaitMode::modeUpdate()"));
#endif
}

