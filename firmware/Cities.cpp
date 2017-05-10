#include "Cities.h"
#include "LED1.h"
#include "LED2.h"
#include <Arduino.h>

Cities_ Cities;

Cities_::Cities_()
{
    // Mapping of LED chunks to city ID - EDIT THIS if
    // you want to re-arrange which city ID maps to which
    // chunk of LEDs...
    // ID     2 3 7 6 1 0 8 9  5 4
    // Label  3 4 8 7 2 1 9 10 6 5 
    // LED    1 1 1 1 1 2 2 2  2 2 
    // Idx    0 1 2 3 4 0 1 2  3 4
    _cities[0].LEDStrip = 2; _cities[0].stripIdx = 0;
    _cities[1].LEDStrip = 1; _cities[1].stripIdx = 4;
    _cities[2].LEDStrip = 1; _cities[2].stripIdx = 0;
    _cities[3].LEDStrip = 1; _cities[3].stripIdx = 1;
    _cities[4].LEDStrip = 2; _cities[4].stripIdx = 4;
    _cities[5].LEDStrip = 2; _cities[5].stripIdx = 3;
    _cities[6].LEDStrip = 1; _cities[6].stripIdx = 3;
    _cities[7].LEDStrip = 1; _cities[7].stripIdx = 2;
    _cities[8].LEDStrip = 2; _cities[8].stripIdx = 1;
    _cities[9].LEDStrip = 2; _cities[9].stripIdx = 2;
}

void Cities_::begin()
{
#ifdef DEBUGDISP
    for (uint8_t i=0; i<CITY_COUNT; i++) {
        Serial.print(F("City id="));
        Serial.print(i);
        Serial.print(F(" LED="));
        Serial.print(_cities[i].LEDStrip);
        Serial.print(F(" Idx="));
        Serial.println(_cities[i].stripIdx);
    }
#endif
    clear();
}

void Cities_::clear()
{
    for (uint8_t i=0; i<CITY_COUNT; i++) {
        _cities[i].wonBy = 0;
    }
    _nextCity = 0;
    display();
}

void Cities_::winCity(uint8_t player)
{
    // sanity check param / state
    if (player<1 || player>2 || _nextCity >= CITY_COUNT) return;
#ifdef DEBUGCITY
    Serial.print(F("winCity #"));
    Serial.print(_nextCity);
    Serial.print(' ');
    Serial.println(player);
#endif
    _cities[_nextCity].wonBy = player;
    _cities[_nextCity].wonMs = millis();
    _nextCity++;
}

void Cities_::display()
{
    for (uint8_t i=0; i<CITY_COUNT; i++) {
#ifdef DEBUGDISP
        Serial.print(F("Display city "));
        Serial.print(i);
        Serial.print(F(" on LED="));
        Serial.println(_cities[i].LEDStrip);
#endif
        uint8_t startPixel = _cities[i].stripIdx*CITY_SIZE;
        for (uint8_t p=0; p<CITY_SIZE; p++) {
#ifdef DEBUGDISP
            Serial.print(F("  pixel "));
            Serial.print(startPixel+p);
            Serial.print(F(" col=0x"));
            Serial.println(getColor(_cities[i].wonBy), HEX);
#endif
            if (_cities[i].LEDStrip==1) {
                LED1.setPixelColor(startPixel+p, getColor(_cities[i].wonBy));
            } else if (_cities[i].LEDStrip==2) {
                LED2.setPixelColor(startPixel+p, getColor(_cities[i].wonBy));
            }
        }
    }
    LED1.show();
    LED2.show();
}

uint8_t Cities_::cityCountForPlayer(uint8_t player)
{
    uint8_t count = 0;
    for (uint8_t i=0; i<CITY_COUNT; i++) {
        if (_cities[i].wonBy == player) {
            count++;
        }
    }
    return count;
}

unsigned long Cities_::getColor(uint8_t player)
{
    switch (player) {
    case 1:
        return P1_ON_COLOR;
        break;
    case 2:
        return P2_ON_COLOR;
        break;
    default:
        return P1_OFF_COLOR;
        break;
    }
}

