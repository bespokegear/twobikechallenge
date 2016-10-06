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
    LEDs.clear();
    LEDs.show();
}

