#include "Cities.h"
#include "LED1.h"
#include "LED2.h"
#include <Arduino.h>

Cities_ Cities;

Cities_::Cities_()
{
    for (uint8_t i=0; i<CITY_COUNT; i++) {
        _cities[i].LEDStrip = cityIDToLEDStrip(i);
        _cities[i].stripIdx = cityIDToStripIdx(i);
    }
}

void Cities_::begin()
{
#ifdef DEBUG
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
    display();
}

void Cities_::setCityWinner(uint8_t cityID, uint8_t player)
{
    if (cityID < CITY_COUNT && player<=2) {
        _cities[cityID].wonBy = player;
    }
}

void Cities_::display()
{
    for (uint8_t i=0; i<CITY_COUNT; i++) {
#ifdef DEBUG
        Serial.print(F("Display city "));
        Serial.print(i);
        Serial.print(F(" on LED="));
        Serial.println(_cities[i].LEDStrip);
#endif
        for (uint8_t p=_cities[i].stripIdx*CITY_SIZE; p<(_cities[i].stripIdx+1)*CITY_SIZE; p++) {
#ifdef DEBUG
            Serial.print(F("  pixel "));
            Serial.print(p);
            Serial.print(F(" col=0x"));
            Serial.println(getColor(_cities[i].wonBy), HEX);
#endif
            if (_cities[i].LEDStrip==1) {
                LED1.setPixelColor(p, getColor(_cities[i].wonBy));
            } else if (_cities[i].LEDStrip==2) {
                LED2.setPixelColor(p, getColor(_cities[i].wonBy));
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

uint8_t Cities_::cityIDToLEDStrip(uint8_t i)
{
    return i < CITY_COUNT/2 ? 1 : 2;
}

uint8_t Cities_::cityIDToStripIdx(uint8_t i)
{
    return i % (CITY_COUNT/2);
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

