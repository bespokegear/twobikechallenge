#pragma once

#include "Mode.h"
#include <stdint.h>

class CountdownMode : public Mode {
public:
    CountdownMode();
    void start();
    void stop();
    void modeUpdate();
    bool isFinished();

private:
    unsigned long _start;
    int8_t seconds();

};

