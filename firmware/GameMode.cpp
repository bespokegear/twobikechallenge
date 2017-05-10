#include "GameMode.h"
#include "Util.h"
#include "Pedal1Vin.h"
#include "Pedal2Vin.h"
#include "LED1.h"
#include "LED2.h"
#include "ClockDisplay.h"
#include "Cities.h"
#include <Arduino.h>
#include <EEPROM.h>

_GameMode GameMode;

_GameMode::_GameMode()
{
    _difficulty = EEPROM.read(EEPROM_LEVEL_ADDRESS);
    if (_difficulty < 1 || _difficulty > GAME_LEVEL_MAX) {
        // bad value from EEPROM
        _difficulty = 1;
    }
}

void _GameMode::begin()
{
#ifdef DEBUG
    Serial.print(F("GameMode::_begin() vinPin1="));
    Serial.print(Pedal1Vin.getPin());
    Serial.print(F(" vinPin2="));
    Serial.println(Pedal2Vin.getPin());
#endif
    start();
}

void _GameMode::start()
{
#ifdef DEBUG
    Serial.println(F("GameMode start"));
#endif
    _energy1 = 0;
    _energy2 = 0;
    _startMillis = millis();
    _lastUpdate = _startMillis;
    _lastLEDUpdate = _startMillis;
#ifdef DEBUG
    _lastDebug = _startMillis;
#endif
    Cities.clear();
    _nextCityEnergy = (Cities.nextCity()+1)*goalEnergy() / CITY_COUNT;
}

void _GameMode::stop()
{
#ifdef DEBUG
    Serial.print(F("GameMode stop"));
#endif
}

void _GameMode::reset()
{
    start();
}

void _GameMode::modeUpdate()
{
    float elapsed = (millis() - _lastUpdate) / 1000.;
    _lastUpdate = millis();
    float vIn1 = PEDAL1_FUDGE_FACTOR + Pedal1Vin.get();
    float vIn2 = PEDAL2_FUDGE_FACTOR + Pedal2Vin.get();
    float power1 = vIn1 > PEDAL1_THRESHOLD ? vIn1*vIn1/PEDAL1_DUMP_R : 0; // P = (V^2)/R
    float power2 = vIn2 > PEDAL2_THRESHOLD ? vIn2*vIn2/PEDAL2_DUMP_R : 0; // P = (V^2)/R
    _energy1 += (power1 * elapsed);
    _energy2 += (power2 * elapsed);
#ifdef DEBUG
    if (_lastUpdate > _lastDebug + 500) {
        _lastDebug = _lastUpdate;
        Serial.print(F("Game elapsed="));
        Serial.print(elapsed);
        Serial.print(F(" pow1="));
        Serial.print(power1);
        Serial.print(F(" pow2="));
        Serial.print(power2);
        Serial.print(F(" e1="));
        Serial.print(_energy1);
        Serial.print(F(" e2="));
        Serial.print(_energy2);
        Serial.print(F(" p1wins="));
        Serial.print(Cities.cityCountForPlayer(1));
        Serial.print(F(" p2wins="));
        Serial.print(Cities.cityCountForPlayer(2));
        Serial.print(F(" City#="));
        Serial.print(Cities.nextCity());
        Serial.print(F(" CityEn="));
        Serial.println(_nextCityEnergy);
    }
#endif
    writeClock();

    // Check to see if we passed the energy necessary to win a city
    if (_energy1 >= _nextCityEnergy || _energy2 >= _nextCityEnergy) {
        Cities.winCity(_energy1 > _energy2 ? 1 : 2);
        _nextCityEnergy = (Cities.nextCity()+1)*goalEnergy() / CITY_COUNT;
    }
    // Throttle writing of neopixels as too-frequent writes
    // throws off millis
    if (_lastUpdate - _lastLEDUpdate > LED_UPDATE_DELAY_MS) {
        Cities.display();
        _lastLEDUpdate = _lastUpdate;
    }
}

void _GameMode::enterBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::enterBrownout"));
#endif
}

void _GameMode::exitBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::exitBrownout"));
#endif
}

void _GameMode::saveToEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::saveToEEPROM"));
#endif
}

void _GameMode::restoreFromEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::restoreFromEEPROM -> "));
#endif
}

bool _GameMode::isFinished()
{
    if ((millis() - _startMillis) > GAME_LENGTH_SECONDS * 1000) {
        switch(getWinner()) {
        case 1:
            ClockDisplay.display("P1!");
            break;
        case 2:
            ClockDisplay.display("P2!");
            break;
        default:
            ClockDisplay.display("1=2");
            break;
        }
        return true;
    } else {
        return false;
    }
}

uint8_t _GameMode::getWinner()
{
    if (Cities.cityCountForPlayer(1) > Cities.cityCountForPlayer(2)) {
        return 1;
    } else if (Cities.cityCountForPlayer(2) > Cities.cityCountForPlayer(1)) {
        return 2;
    } else {
        // equal number of cities - decide by energy
        if (_energy1 > _energy2) {
            return 1;
        } else if (_energy2 > _energy1) {
            return 2;
        } else {
            // both city count and total energy the same - draw!
            return 0;
        }
    }
}

void _GameMode::writeClock()
{
    long left10ths = ((_startMillis + GAME_LENGTH_SECONDS * 1000) - millis())/100;
    if (left10ths == _lastClock) { return; }
    uint8_t c1 = (left10ths / 100) % 10;
    uint8_t c2 = (left10ths / 10) % 10;
    uint8_t c3 = left10ths % 10;
    _lastClock = left10ths;
    ClockDisplay.display(c1==0 ? ' ' : c1, c2, c3, 2);
}

void _GameMode::setLevel(uint8_t d)
{
    _difficulty = d;
}

uint8_t _GameMode::getLevel()
{
    return _difficulty;
}

float _GameMode::goalEnergy() 
{
    return _difficulty * GAME_LEVEL_ENERGY_STEP;
}

