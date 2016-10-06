#pragma once

#include "Mode.h"

class WaitMode : public Mode {
public:
    WaitMode();
    void start();
    void stop();
    void modeUpdate();

};

