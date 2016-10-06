#pragma once

#include <Arduino.h>

// General config
#define PEDAL1_VOLTAGE_PIN          A0
#define PEDAL1_VOLTAGE_R1           10
#define PEDAL1_VOLTAGE_R2           560
#define PEDAL2_VOLTAGE_PIN          A1
#define PEDAL2_VOLTAGE_R1           10
#define PEDAL2_VOLTAGE_R2           560
#define ARDUINO_VOLTAGE_PIN         A6
#define ARDUINO_VOLTAGE_R1          10
#define ARDUINO_VOLTAGE_R2          100
#define RESET_BUTTON_PIN            2
#define HEARTBEAT_LED_PIN           13
#define INDICATOR_LED_PIN           9
#define BROWNOUT_HIGH               1300
#define BROWNOUT_LOW                300
#define VOLTAGE_SAMPLES             20
#define SLATCH_PIN                  10
#define SCLK_PIN                    11
#define SDATA_PIN                   12

// LED strip parameters
// This is for initialising the Adafruit_NeoPixel library
// and will depend on the LED type
#define LED1_TYPE                   (NEO_GRB + NEO_KHZ800)
#define LED1_DATA_PIN               3
#define LED1_COUNT                  109
#define LED2_TYPE                   (NEO_GRB + NEO_KHZ800)
#define LED2_DATA_PIN               4
#define LED2_COUNT                  109

// Brightness is a range of 0-255, 255 being brightest
// It will set the brightness in all display modes
#define LED_BRIGHTNESS              16
// This is used to compensate for the time it takes to write to 
// the LED1 - without it, longer strips would take longer to 
// fill based on the VOLT_MODE_FASTEST_SEC setting.  Determined
// by trial and error
#define LED_SPEED_FACTOR            180

// GameMode parameters
#define P1_ON_COLOR                 0xFF0000UL
#define P1_OFF_COLOR                0x000000UL
#define P2_ON_COLOR                 0x00FF00UL
#define P2_OFF_COLOR                0x000000UL
// in WattSeconds
#define P1_GOAL_WS                  10000
#define P2_GOAL_WS                  10000
#define GAME_LENGTH_SECONDS         30

// Other mode settings
#define COUNTDOWN_SECONDS           3

