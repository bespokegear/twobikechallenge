#pragma once

#include <stdint.h>
#include "Config.h"

struct City
{
    uint8_t LEDStrip;  // City is on LEDx
    uint8_t stripIdx;  // City position on specified strip
    uint8_t wonBy;     // City currently won by player wonBy

};

class Cities_ {
public:
    // Construct cities data (which pixels for which city)
    Cities_();

    // Just dumps city details in DEBUG mode
    void begin();

    // Clear city win state
    void clear();  

    // Set a city color to be won by specified player
    void winCity(uint8_t player);

    // Update display and send data to LEDs
    void display();

    // Get how many cites are currently won by specified player
    uint8_t cityCountForPlayer(uint8_t player);

    // Get the next city number
    uint8_t nextCity() { return _nextCity; }

private:
    City _cities[CITY_COUNT];
    uint8_t _nextCity;

    unsigned long getColor(uint8_t player);

};

extern Cities_ Cities;

