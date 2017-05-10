#pragma once

#include <Arduino.h>

// General config
#define PEDAL1_VOLTAGE_PIN          A1
#define PEDAL1_VOLTAGE_R1           10
#define PEDAL1_VOLTAGE_R2           560
#define PEDAL1_DUMP_R               4.0
#define PEDAL1_FUDGE_FACTOR         2.1
#define PEDAL1_THRESHOLD            PEDAL1_FUDGE_FACTOR + 0.2
#define PEDAL2_VOLTAGE_PIN          A0
#define PEDAL2_VOLTAGE_R1           10
#define PEDAL2_VOLTAGE_R2           560
#define PEDAL2_DUMP_R               4.0
#define PEDAL2_FUDGE_FACTOR         2.1
#define PEDAL2_THRESHOLD            PEDAL2_FUDGE_FACTOR + 0.2
#define ARDUINO_VOLTAGE_PIN         A6
#define ARDUINO_VOLTAGE_R1          10
#define ARDUINO_VOLTAGE_R2          100
#define RESET_BUTTON_PIN            2
#define MODE_BUTTON_PIN             5
#define RESET_RADIO_PIN             A4
#define MODE_RADIO_PIN              A5
#define HEARTBEAT_LED_PIN           13
#define INDICATOR_LED_PIN           9
#define BROWNOUT_HIGH               1300
#define BROWNOUT_LOW                300
#define VOLTAGE_SAMPLES             20
#define SLATCH_PIN                  10
#define SCLK_PIN                    11
#define SDATA_PIN                   12
#define EEPROM_LEVEL_ADDRESS        0

// Button feel settings
#define DEBOUNCED_BUTTON_THRESHOLD      5
#define DEBOUNCED_BUTTON_DELAY          5
#define DEBOUNCED_BUTTON_HELD_MS        800
#define DEBOUNCED_BUTTON_RPT_INITIAL_MS 1000
#define DEBOUNCED_BUTTON_RPT_MS         300

#define CITY_COUNT                  10
// Size of city in LEDs
#define CITY_SIZE                   24

// LED strip parameters
// This is for initialising the Adafruit_NeoPixel library
// and will depend on the LED type
#define LED1_TYPE                   (NEO_GRB + NEO_KHZ800)
#define LED1_DATA_PIN               3
#define LED1_COUNT                  (CITY_SIZE*(CITY_COUNT/2))
#define LED1_BRIGHTNESS             255
#define LED2_TYPE                   (NEO_GRB + NEO_KHZ800)
#define LED2_DATA_PIN               4
#define LED2_COUNT                  ((CITY_COUNT-(CITY_COUNT/2))*CITY_SIZE)
#define LED2_BRIGHTNESS             255
#define LED_UPDATE_DELAY_MS         100

// GameMode parameters
#define P1_ON_COLOR                 0xFF0000UL
#define P1_OFF_COLOR                0x000000UL
#define P2_ON_COLOR                 0x0000FFUL
#define P2_OFF_COLOR                0x000000UL
#define GAME_LEVEL_ENERGY_STEP      1000
#define GAME_LEVEL_MAX              20
#define GAME_LENGTH_SECONDS         30

// Other mode settings
#define COUNTDOWN_SECONDS           3

