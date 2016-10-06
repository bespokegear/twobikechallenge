#include "GameMode.h"
#include "Util.h"
#include "PedalVoltage.h"
#include <Arduino.h>
#include <EEPROM.h>

GameMode::GameMode() :
    DisplayMode()
{
#ifdef DEBUG
    Serial.print(F("GameMode::GameMode(), voltPin="));
    Serial.println(PedalVoltage.getPin());
#endif
    // get _lastPixel from EEPROM
    restoreFromEEPROM();
    _lastUpdate = millis();
}

void GameMode::start()
{
#ifdef DEBUG
    Serial.println(F("GameMode start"));
#endif
    // fetch _lastPixel value from EEPROM
    _lastPixel = 0;
    DisplayMode::start();
    if (!isBrownedOut()) {
        writePixels();
    }
}

void GameMode::stop()
{
#ifdef DEBUG
    Serial.print(F("GameMode stop"));
#endif
    _lastPixel = 0;
    DisplayMode::stop();
}

void GameMode::reset()
{
    start();
    // If we're browned out when reset is pressed, the reset value would be
    // over-written when we come out of brown out and the lastPixel value is
    // restored from EEPROM, so we will save to EEPROM after reset if we are
    // presently in brownout mode...
    if (isBrownedOut()) {
#ifndef NOEEPROM
        saveToEEPROM(); 
#endif
    }
}


void GameMode::modeUpdate()
{
    float elapsed = (millis() - _lastUpdate) / 1000.;
    _lastUpdate = millis();
    // When we're browned out, keep writing those off pixels
    if (isBrownedOut()) {
        LEDs.clear();
        LEDs.show();
    }
    uint16_t vIn = PedalVoltage.get();
#ifdef DEBUGVIN
    Serial.print(F("vIn="));
    Serial.print(vIn);
    Serial.print(F(", elapsed="));
    Serial.print(elapsed);
#endif
    if (vIn < VOLT_MODE_VMIN) {
        vIn = VOLT_MODE_VMIN;
    } else if (vIn > VOLT_MODE_VMAX) {
        vIn = VOLT_MODE_VMAX;
    }
#ifdef DEBUGVIN
    Serial.print(F(", clipped="));
    Serial.print(vIn);
#endif
    uint16_t prevInt = (uint16_t)_lastPixel;
    float linear = (float)(vIn-VOLT_MODE_VMIN) / (VOLT_MODE_VMAX-VOLT_MODE_VMIN);
    float nonLinear = calculateLinearity(linear, VOLT_MODE_LINEARITY);
    if (_lastPixel + nonLinear < NUMBER_OF_LEDS+1) {
        // The -(float)NUMBER_OF_LEDS/LED_SPEED_FACTOR) is to compensate for time taken sending data on longer strings
        _lastPixel += nonLinear * (NUMBER_OF_LEDS*elapsed/(VOLT_MODE_FASTEST_SEC-(float)NUMBER_OF_LEDS/LED_SPEED_FACTOR));
    }
    // have we gone up past another whole number?
#ifdef DEBUGVIN
    Serial.print(F(", lin="));
    Serial.print(linear);
    Serial.print(F(", nlin="));
    Serial.print(nonLinear);
    Serial.print(F(", lp="));
    Serial.println(_lastPixel);
#endif
    if ((uint16_t)_lastPixel > prevInt && (uint16_t)_lastPixel <= LEDs.numPixels()) {
#ifdef DEBUG
        Serial.print(F("+pixel, lp="));
        Serial.println(_lastPixel);
#endif
        writePixels();
    }
}

void GameMode::enterBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::enterBrownout"));
#endif
    LEDs.clear();
    LEDs.show();
    // saves _lastPixel;
#ifndef NOEEPROM
    saveToEEPROM(); 
#endif
}

void GameMode::exitBrownout()
{
#ifdef DEBUG
    Serial.println(F("GameMode::exitBrownout"));
#endif
    // restore _lastPixel
#ifndef NOEEPROM
    restoreFromEEPROM(); 
#endif
    // restore pixels on LED strip
    writePixels();
}

void GameMode::saveToEEPROM()
{
#ifdef DEBUG
    Serial.print(F("GameMode::saveToEEPROM <- "));
    Serial.println(_lastPixel);
#endif
    uint16_t lp = _lastPixel;
    uint8_t* ptr = (uint8_t*)(&lp);
    EEPROM.write(VOLT_MODE_EEPROM_OFFSET+0, ptr[0]);
    EEPROM.write(VOLT_MODE_EEPROM_OFFSET+1, ptr[1]);
}

void GameMode::restoreFromEEPROM()
{
    uint16_t lp;
    uint8_t* ptr = (uint8_t*)(&lp);
    ptr[0] = EEPROM.read(0);
    ptr[1] = EEPROM.read(1);
    if (lp > NUMBER_OF_LEDS) {
        lp = NUMBER_OF_LEDS;
    }
    _lastPixel = lp;
#ifdef DEBUG
    Serial.print(F("GameMode::restoreFromEEPROM -> "));
    Serial.println(_lastPixel);
#endif
}

void GameMode::writePixels()
{
#ifdef DEBUG
    Serial.println(F("GameMode::writePixels"));
#endif
    for (uint16_t i=0; i<LEDs.numPixels(); i++) {
        if (i < _lastPixel-1) {
            LEDs.setPixelColor(i, VOLT_MODE_COLOR_ON);
        } else {
            LEDs.setPixelColor(i, VOLT_MODE_COLOR_OFF);
        }
    }
    LEDs.show();
}
