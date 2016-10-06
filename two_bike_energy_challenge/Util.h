#pragma once

#include <stdint.h>

float highVoltageConversion(const uint8_t pin);
float voltageConversion(const uint8_t pin, const uint16_t r1KOhm, const uint16_t r2KOhm);
float calculateLinearity(float percent, float linearity);

// From the MemoryFree library, https://github.com/maniacbug/MemoryFree
#ifdef __cplusplus
extern "C" {
#endif

int freeMemory();

#ifdef  __cplusplus
}
#endif


