#pragma once

#include "Config.h"
#include "DisplayMode.h"
#include <stdint.h>

class GameMode : public DisplayMode {
public:
    GameMode();
              
    virtual void start();
    virtual void stop();
    virtual void reset();
    virtual void modeUpdate();
    virtual void enterBrownout();
    virtual void exitBrownout();

private:
    float _lastPixel;
    unsigned long _lastUpdate;

    void saveToEEPROM();
    void restoreFromEEPROM();

    void writePixels();
};

