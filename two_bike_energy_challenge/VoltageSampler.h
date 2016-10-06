#pragma once

#include "Config.h"
#include <stdint.h>

class VoltageSampler {
public:
    VoltageSampler(const uint8_t pin, uint16_t r1KOhm, uint16_t r2KOhm);
    ~VoltageSampler() {;}

    void update();
    uint16_t get();

    uint8_t getPin() { return _pin; }

private:
    uint8_t _pin;
    uint16_t _r1KOhm;
    uint16_t _r2KOhm;
    uint8_t _count;
    uint8_t _idx;
    uint16_t _lastAvg;
    bool _updated;
    uint16_t _samples[VOLTAGE_SAMPLES];

};

