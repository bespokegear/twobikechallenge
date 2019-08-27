#pragma once

#include "Config.h"
#include "Mode.h"
#include <stdint.h>

class _GameMode : public Mode {
public:
    _GameMode();
    void begin();
    void start();
    void stop();
    void reset();
    void modeUpdate();
    void enterBrownout();
    void exitBrownout();
    bool isFinished();
    void saveToEEPROM();
    void restoreFromEEPROM();
    void writePixels();
    void writeClock();
    void setLevel(uint8_t d);
    uint8_t getLevel();

private:
    unsigned long _startMillis;
    unsigned long _lastUpdate;
    unsigned long _powerUpdate;
    unsigned long _lastLEDUpdate;
    long _lastClock;
    float _energy1;
    float _energy2;
    uint8_t _difficulty;

    float goalEnergy();
    uint16_t old_lights_lit;
    uint16_t total_lights_lit;
    bool finish_flag;
};

extern _GameMode GameMode;
