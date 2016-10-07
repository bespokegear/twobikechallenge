// Even though these libraries are only used in other sources, we
// #include them here so the Ardino IDE knows to compile and link 
// them...
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "Heartbeat.h"
#include "DebouncedButton.h"
#include "WaitMode.h"
#include "CountdownMode.h"
#include "GameMode.h"
#include "Util.h"
#include "LEDs.h"
#include "VinMonitors.h"
#include "ClockDisplay.h"
//#include "LEDs.h"
#include <Arduino.h>
#include <avr/wdt.h>


// See Config.h for pin and other configuration

// Global variables

Heartbeat* heartbeat;
DebouncedButton* resetButton;
Mode* mode = &WaitMode;

#ifdef DEBUGTIME
unsigned long lastLoop = 0;
#endif

void setup()
{
    Serial.begin(115200);

    // Use external reference
    analogReference(EXTERNAL);

    // Set up the blinker
    heartbeat = new Heartbeat(HEARTBEAT_LED_PIN);

    // Construct input buttons (sets pin modes in constructor)
    resetButton = new DebouncedButton(RESET_BUTTON_PIN);

    // init pins for clock display
    ClockDisplay.begin();
    ClockDisplay.clear();

    // Ensure load is disconnected at start, indicator off
    pinMode(INDICATOR_LED_PIN, OUTPUT);
    digitalWrite(INDICATOR_LED_PIN, LOW);

    // Configure global Adafruit_Neopixel objects (from LEDs.h)
    LED1.begin();
    LED2.begin();
    LED1.setBrightness(LED1_BRIGHTNESS);
    LED2.setBrightness(LED2_BRIGHTNESS);
    LED1.clear();
    LED2.clear();
    LED1.show(); // Initialize all pixels to 'off'
    LED2.show(); // Initialize all pixels to 'off'

    // init various modes
    WaitMode.begin();
    CountdownMode.begin();
    GameMode.begin();

    // Let things settle
    delay(500);

    // Call start for current mode
    mode->start();

    // Enable watchdog reset at 1/4 sec
    wdt_enable(WDTO_250MS);

#ifdef DEBUG
    Serial.println(F("setup() E"));
#endif
}

void loopDebug()
{
#ifdef DEBUG
    if (mode == &WaitMode) {
        Serial.println(F("in WaitMode"));
    }
    else if (mode == &CountdownMode) {
        Serial.println(F("in CountdownMode"));
    }
    else if (mode == &GameMode) {
        Serial.println(F("in GameMode"));
    }
    else {
        Serial.println(F("Unknown mode!"));
    }
#endif 

#ifdef DEBUGTIME
    Serial.print(F("looptime="));
    Serial.println(millis() - lastLoop);
    lastLoop = millis();
#endif

#ifdef DEBUGVIN
    Serial.print(F("Vin p1="));
    Serial.print(PedalVoltage1.get());
    Serial.print(F(" p2="));
    Serial.print(PedalVoltage2.get());
    Serial.print(F(" ard="));
    Serial.println(ArduinoVoltage.get());
#endif

#ifdef DEBUGMEM
    Serial.print(F("free="));
    Serial.println(freeMemory());
#endif
}

void switchMode(Mode* newMode)
{
    mode->stop();
    mode = newMode;
    mode->start();
}

void loop()
{
    // Feed the watchdog
    wdt_reset();

    // Give a time slice to various peripheral functions
    heartbeat->update();
    resetButton->update();
    PedalVoltage1.update();
    PedalVoltage2.update();
    ArduinoVoltage.update();

    // Dump debugging to serial (if #defined)
    loopDebug();

    // Detect button presses and behave appropriately
    if (resetButton->isPressed()) {
#ifdef DEBUG
        Serial.println(F("BUTTON: starting countdown"));
#endif
        switchMode(&CountdownMode);
    }

    // Give a timeslice to the current mode
    mode->update();

    // Handle modes timing out
    if (mode->isFinished()) {
        if (mode == &CountdownMode) { switchMode(&GameMode); }
        else if (mode == &GameMode) { switchMode(&WaitMode); }
    }
}

