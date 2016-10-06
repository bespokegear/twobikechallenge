#include "Config.h"
#include "CountdownMode.h"
#include "ClockDisplay.h"
#include <stdint.h>
#include <Arduino.h>

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
    _lastSent = -1;
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
    int8_t left = seconds();
#ifdef DEBUG
    Serial.print(F("CountdownMode::modeUpdate() left="));
    Serial.println(left);
#endif
    if (_lastSent != left) {
        _lastSent = left;
        if (left > 0) {
            ClockDisplay.display(left);
        } else {
            ClockDisplay.display("Go!");
        }
    }
}

int8_t CountdownMode::seconds()
{
    return COUNTDOWN_SECONDS - ((millis()-_start)/1000);
}

bool CountdownMode::isFinished()
{
    return seconds() < 0;
}

