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
    _energy1 = 0.;
    _energy2 = 0.;
    _startMillis = millis();
    _lastUpdate = _startMillis;
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
    uint16_t vIn1 = pedalVoltage1.get();
    uint16_t vIn2 = pedalVoltage2.get();
#ifdef DEBUG
    Serial.print(F("elapsed="));
    Serial.print(elapsed);
    Serial.print(F(" vIn1="));
    Serial.print(vIn1);
    Serial.print(F(" vIn2="));
    Serial.println(vIn2);
#endif
    writeClock();
    writePixels();
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
}

bool GameMode::isFinished()
{
    return (millis() - _startMillis) > GAME_LENGTH_SECONDS * 1000;
}

void GameMode::writeClock()
{
    long left10ths = ((_startMillis + GAME_LENGTH_SECONDS * 1000) - millis())/100;
#ifdef DEBUG
    Serial.print(F("GameMode::writeClock left="));
    Serial.println(left10ths);
#endif
    ClockDisplay.display(left10ths);
}

