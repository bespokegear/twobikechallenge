#include "GameMode.h"
#include "Util.h"
#include "Pedal1Vin.h"
#include "Pedal2Vin.h"
#include "LED1.h"
#include "LED2.h"
#include "ClockDisplay.h"
#include <Arduino.h>
#include <EEPROM.h>

_GameMode GameMode;

_GameMode::_GameMode()
{
    _difficulty = EEPROM.read(EEPROM_LEVEL_ADDRESS);
    if (_difficulty < 1 || _difficulty > GAME_LEVEL_MAX) {
        // bad value from EEPROM
        _difficulty = 1;
    }
}

void _GameMode::begin()
{
#ifdef DEBUG
    Serial.print(F("GameMode::_begin() vinPin1="));
    Serial.print(Pedal1Vin.getPin());
    Serial.print(F(" vinPin2="));
    Serial.println(Pedal2Vin.getPin());
#endif
    start();
}

void _GameMode::start()
{
#ifdef DEBUG
    Serial.println(F("GameMode start"));
#endif
    _energy1 = 0;
    _energy2 = 0;
    _startMillis = millis();
    _lastUpdate = _startMillis;
    _lastLEDUpdate = _startMillis;
    finish_flag=false;
    writePixels();
}

void _GameMode::stop()
{
#ifdef DEBUG
    Serial.print(F("GameMode stop"));
#endif
}

void _GameMode::reset()
{
    start();
}

void _GameMode::modeUpdate()
{
    float elapsed = (millis() - _lastUpdate) / 1000.;
    _lastUpdate = millis();
      
    float vIn1 = PEDAL1_FUDGE_FACTOR + Pedal1Vin.get();
    float vIn2 = PEDAL2_FUDGE_FACTOR + Pedal2Vin.get();
    float power1 = vIn1 > PEDAL1_THRESHOLD ? vIn1*vIn1/PEDAL1_DUMP_R : 0; // P = (V^2)/R
    float power2 = vIn2 > PEDAL2_THRESHOLD ? vIn2*vIn2/PEDAL2_DUMP_R : 0; // P = (V^2)/R
    _energy1 += (power1 * elapsed);
    _energy2 += (power2 * elapsed);
    
#ifdef DEBUG
    Serial.print(F("Game elapsed="));
    Serial.print(elapsed);
    Serial.print(F(" pow1="));
    Serial.print(power1);
    Serial.print(F(" pow2="));
    Serial.print(power2);
    Serial.print(F(" e1="));
    Serial.print(_energy1);
    Serial.print(F(" e2="));
    Serial.println(_energy2);
#endif
    writeClock();
    // Throttle writing of neopixels as too-frequent writes
    // throws off millis
    if (_lastUpdate - _lastLEDUpdate > LED_UPDATE_DELAY_MS) {
        writePixels();
        _lastLEDUpdate = _lastUpdate;
    }
    // Here I want to display the power and energy totals
    // Use power 1 + power 2 and energy 1 + energy 2
    // Only want to do this every 500mS or so
    if  (millis()>(_powerUpdate + POWER_DISPLAY_MS) || finish_flag==true){
      _powerUpdate = millis();
      #ifdef DEBUG_MINIMAL
        Serial.print(F("Time="));
        Serial.print((millis()- _startMillis)/1000.);
        Serial.print(F(" P="));
        Serial.print(power1+power2);
        Serial.print(F(" E="));
        Serial.println(_energy1+_energy2);  
      #endif
      #ifdef JSON_OUTPUT 
        Serial.print(F("{\"box\":"));   
        Serial.print(BOX_ID);
        Serial.print(F(",\"Time\":"));           
        Serial.print((millis()- _startMillis)/1000.);
        Serial.print(F(",\"Power\":"));
        Serial.print(power1+power2);
        Serial.print(F(",\"Energy\":"));
        Serial.print(_energy1+_energy2);       
        Serial.print(F(",\"Lights\":"));
        Serial.print(total_lights_lit);  
        Serial.println("}");
      #endif     
    } 
}

void _GameMode::enterBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::enterBrownout"));
#endif
}

void _GameMode::exitBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::exitBrownout"));
#endif
}

void _GameMode::saveToEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::saveToEEPROM"));
#endif
}

void _GameMode::restoreFromEEPROM()
{
#ifdef DEBUG
    Serial.println(F("GameMode::restoreFromEEPROM -> "));
#endif
}

void _GameMode::writePixels()
{
#ifdef DEBUG
    Serial.println(F("GameMode::writePixels"));
#endif
    uint16_t i;
    bool lit;
    uint16_t lights_lit;
    
    // Just using LED1 to show both energy together
    lights_lit = 0;
    for (i=1; i<LED1_COUNT+1; i++) {
        bool lit = (((_energy1+_energy2)*LED1_COUNT) / goalEnergy()) > i;
        if(lit == 1){
          lights_lit++;
        }
        for (int n = LED_BLOCK; n>=0 ; n--)
        {
           LED1.setPixelColor(((i-1)*LED_BLOCK)+n, lit ? P1_ON_COLOR : P1_OFF_COLOR);
        }
    }
    LED1.show();   
    // This section outputs the lights to be lit
    if(lights_lit != old_lights_lit) {
      #ifdef DEBUG_MINIMAL
        Serial.print(F("Light:"));
        Serial.println(lights_lit);
      #endif 
      old_lights_lit = lights_lit;
      total_lights_lit = lights_lit;
    }    
}

bool _GameMode::isFinished()
{
    // Game is finished when energy is greater than energy max
    if ( (_energy1 + _energy2) > (goalEnergy()+5) ) {
        // Enter here when game finished
        finish_flag=true;
        modeUpdate();
        // Serial print the energy from each player
        #ifdef DEBUG
          Serial.print(F("P1 Energy: "));
          Serial.print(_energy1);
          Serial.print(F(" P2 Energy: "));
          Serial.println(_energy2);
        #endif   
             
        #ifdef DEBUG_MINIMAL
          // Output data for analysis
          Serial.print(F("P1 Energy: "));
          Serial.print(_energy1);
          Serial.print(F(" P2 Energy: "));
          Serial.println(_energy2);  
        #endif
           
        old_lights_lit = 0;
        total_lights_lit = 0;
        return true;
    } else {
        return false;
    }
}

void _GameMode::writeClock()
{
    long left10ths = ((_startMillis + GAME_LENGTH_SECONDS * 1000) - millis())/100;
    if (left10ths == _lastClock) { return; }
    uint8_t c1 = (left10ths / 100) % 10;
    uint8_t c2 = (left10ths / 10) % 10;
    uint8_t c3 = left10ths % 10;
    _lastClock = left10ths;
    ClockDisplay.display(c1==0 ? ' ' : c1, c2, c3, 2);
}

void _GameMode::setLevel(uint8_t d)
{
    _difficulty = d;
}

uint8_t _GameMode::getLevel()
{
    return _difficulty;
}

float _GameMode::goalEnergy() 
{
    return _difficulty * GAME_LEVEL_ENERGY_STEP;
}
