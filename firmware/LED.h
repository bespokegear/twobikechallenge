#pragma once
#include <Adafruit_NeoPixel.h>

// This class does everything a regular Adafruit_NeoPixel object does
// but also sets the millis offset when show() is called.
class LED : public Adafruit_NeoPixel {
public:
    LED(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800);
    void show(void);

};
