#include "CountdownMode.h"
#include <stdint.h>
#include <Arduino.h>

#define COUNTDOWN_SECONDS 5

CountdownMode::CountdownMode()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::CountdownMode()"));
#endif
    start();
}

void CountdownMode::start()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::start()"));
#endif
    _start = millis();
}

void CountdownMode::stop()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::stop()"));
#endif
    _start = 0;
}

void CountdownMode::modeUpdate()
{
#ifdef DEBUG
    Serial.print(F("CountdownMode::modeUpdate() left="));
    long left = COUNTDOWN_SECONDS - ((millis()-_start)/1000);
    Serial.println(left);
#endif
}

bool CountdownMode::isFinished()
{
    return millis() - _start >= (COUNTDOWN_SECONDS*1000);
}

