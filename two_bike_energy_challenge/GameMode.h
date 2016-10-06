#pragma once

#include "Config.h"
#include "DisplayMode.h"
#include <stdint.h>

class GameMode : public Mode {
public:
    GameMode();
              
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

private:
    unsigned long _startMillis;
    unsigned long _lastUpdate;
    unsigned long _lastLEDUpdate;
    long _lastClock;
    float _energy1;
    float _energy2;
    float _goal1;
    float _goal2;

};

