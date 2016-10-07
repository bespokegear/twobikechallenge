#include "GameMode.h"
#include "Util.h"
#include "VinMonitors.h"
#include "LEDs.h"
#include "ClockDisplay.h"
#include <Arduino.h>
#include <EEPROM.h>

GameMode::GameMode() :
    _goal1(P1_GOAL_WS),
    _goal2(P2_GOAL_WS)
{
#ifdef DEBUG
    Serial.print(F("GameMode::GameMode() vinPin1="));
    Serial.print(pedalVoltage1.getPin());
    Serial.print(F(" vinPin2="));
    Serial.println(pedalVoltage2.getPin());
#endif
    start();
}

void GameMode::start()
{
#ifdef DEBUG
    Serial.println(F("GameMode start"));
#endif
    _energy1 = 0;
    _energy2 = 0;
    _startMillis = millis();
    _lastUpdate = _startMillis;
    _lastLEDUpdate = _startMillis;
    writePixels();
}

void GameMode::stop()
{
#ifdef DEBUG
    Serial.print(F("GameMode stop"));
#endif
}

void GameMode::reset()
{
    start();
}

void GameMode::modeUpdate()
{
    float elapsed = (millis() - _lastUpdate) / 1000.;
    _lastUpdate = millis();
    float vIn1 = PEDAL1_FUDGE_FACTOR + pedalVoltage1.get();
    float vIn2 = PEDAL2_FUDGE_FACTOR + pedalVoltage2.get();
    float power1 = vIn1 > PEDAL1_THRESHOLD ? vIn1*vIn1/PEDAL1_DUMP_R : 0; // P = (V^2)/R
    float power2 = vIn2 > PEDAL2_THRESHOLD ? vIn2*vIn2/PEDAL2_DUMP_R : 0; // P = (V^2)/R
    _energy1 += (power1 * elapsed);
    _energy2 += (power2 * elapsed);
#ifdef DEBUGVIN
    Serial.print(F("elapsed="));
    Serial.print(elapsed);
    Serial.print(F(" vIn1="));
    Serial.print(vIn1);
    Serial.print(F(" vIn2="));
    Serial.print(vIn2);
    Serial.print(F(" pow1="));
    Serial.print(power1);
    Serial.print(F(" pow2="));
    Serial.print(power2);
    Serial.print(F(" e1="));
    Serial.print(_energy1);
    Serial.print(F(" e2="));
    Serial.println(_energy2);
#endif
    writeClock();
    // Throttle writing of neopixels as too-frequent writes
    // throws off millis
    if (_lastUpdate - _lastLEDUpdate > LED_UPDATE_DELAY_MS) {
        writePixels();
        _lastLEDUpdate = _lastUpdate;
    }
}

void GameMode::enterBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::enterBrownout"));
#endif
}

void GameMode::exitBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::exitBrownout"));
#endif
}

void GameMode::saveToEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::saveToEEPROM"));
#endif
}

void GameMode::restoreFromEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::restoreFromEEPROM -> "));
#endif
}

void GameMode::writePixels()
{
#ifdef DEBUG
    Serial.println(F("GameMode::writePixels"));
#endif
    uint16_t i;
    bool lit;
    for (i=0; i<LED1_COUNT; i++) {
        bool lit = ((_energy1*LED1_COUNT) / _goal1) > i;
        LED1.setPixelColor(i, lit ? P1_ON_COLOR : P1_OFF_COLOR);
    }
    for (i=0; i<LED2_COUNT; i++) {
        bool lit = ((_energy2*LED2_COUNT) / _goal2) > i;
        LED2.setPixelColor(i, lit ? P2_ON_COLOR : P2_OFF_COLOR);
    }
    LED1.show();
    LED2.show();
}

bool GameMode::isFinished()
{
    if ((millis() - _startMillis) > GAME_LENGTH_SECONDS * 1000) {
        if (_energy1 > _energy2) {
            ClockDisplay.display("P1!");
        } else if (_energy2 > _energy1) {
            ClockDisplay.display("P2!");
        } else {
            ClockDisplay.display("1=2");
        }
        return true;
    } else {
        return false;
    }
}

void GameMode::writeClock()
{
    long left10ths = ((_startMillis + GAME_LENGTH_SECONDS * 1000) - millis())/100;
    if (left10ths == _lastClock) { return; }
    uint8_t c1 = (left10ths / 100) % 10;
    uint8_t c2 = (left10ths / 10) % 10;
    uint8_t c3 = left10ths % 10;
    _lastClock = left10ths;
    ClockDisplay.display(c1==0 ? ' ' : c1, c2, c3, 2);
}

