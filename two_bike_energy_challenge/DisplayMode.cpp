#include "DisplayMode.h"

DisplayMode::DisplayMode()
{
    resetPixels();
}

DisplayMode::~DisplayMode()
{
    resetPixels();
}

void DisplayMode::start()
{
    resetPixels();   
}

void DisplayMode::stop()
{
    resetPixels();   
}

void DisplayMode::resetPixels()
{
    LED1.clear();
    LED1.show();
}

