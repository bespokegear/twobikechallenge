#include "VinMonitors.h"
#include "Config.h"

VoltageSampler pedalVoltage1(PEDAL1_VOLTAGE_PIN, PEDAL1_VOLTAGE_R1, PEDAL1_VOLTAGE_R2);
VoltageSampler pedalVoltage2(PEDAL2_VOLTAGE_PIN, PEDAL2_VOLTAGE_R1, PEDAL2_VOLTAGE_R2);
VoltageSampler ArduinoVoltage(ARDUINO_VOLTAGE_PIN, ARDUINO_VOLTAGE_R1, ARDUINO_VOLTAGE_R2);

