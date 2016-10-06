#pragma once

#include "Mode.h"

class CountdownMode : public Mode {
public:
    CountdownMode();
    void start();
    void stop();
    void modeUpdate();
    bool isFinished();

private:
    unsigned long _start;

};

