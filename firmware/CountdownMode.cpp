#include "Config.h"
#include "CountdownMode.h"
#include "ClockDisplay.h"
#include "LED1.h"
#include "LED2.h"
#include <stdint.h>
#include <Arduino.h>

_CountdownMode CountdownMode;

_CountdownMode::_CountdownMode()
{
}

void _CountdownMode::begin()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::begin()"));
#endif
    start();
}

void _CountdownMode::start()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::start()"));
#endif
    _start = millis();
    _lastSent = -1;
}

void _CountdownMode::stop()
{
#ifdef DEBUG
    Serial.println(F("CountdownMode::stop()"));
#endif
    _start = 0;
}

void _CountdownMode::modeUpdate()
{
    uint16_t i;
    int8_t left = seconds();
#ifdef DEBUG
    Serial.print(F("CountdownMode::modeUpdate() left="));
    Serial.println(left);
#endif
    if (_lastSent != left) {
        _lastSent = left;
        if (left > 0) {
            ClockDisplay.display(' ', left, ' ');
            #ifdef DEBUG_MINIMAL
              Serial.print(F("Countdown: "));
              Serial.println(left);
            #endif
              
            // Here the count down happens in red lights at the base of the LEDs
            for (i=0; i<LED1_COUNT; i++) {
                bool lit = left > i;
                for (int n = LED_BLOCK; n>=0 ; n--)
                {
                  LED1.setPixelColor((i*LED_BLOCK)+n, lit ? COUNTDOWN_COLOUR : P1_OFF_COLOR);
                }
            }
            LED1.show(); 
            #ifdef JSON_OUTPUT 
              Serial.print(F("{\"box\":"));   
              Serial.print(BOX_ID);
              Serial.print(F(",\"Countdown\":"));           
              Serial.print(left);
              Serial.println("}");
            #endif 

                
        } else {
            ClockDisplay.display("Go!");
             #ifdef DEBUG_MINIMAL
                Serial.println(F("Go!"));
             #endif 
            for (int n = LED_BLOCK; n>=0 ; n--)
            {
                LED1.setPixelColor(n, P1_OFF_COLOR);
            }
            LED1.show();      
        }
    }
}

int8_t _CountdownMode::seconds()
{
    return COUNTDOWN_SECONDS - ((millis()-_start)/1000);
}

bool _CountdownMode::isFinished()
{
    return seconds() < 0;
}
