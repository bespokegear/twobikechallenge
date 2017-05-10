#include "LED.h"
#include "Millis.h"

LED::LED(uint16_t n, uint8_t p, neoPixelType t=NEO_GRB + NEO_KHZ800) :
    Adafruit_NeoPixel(n, p, t)
{
}

void LED::show()
{
    Adafruit_NeoPixel::show();
    addMillisOffset(numPixels() * 0.0215);
}



