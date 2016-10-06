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
#include "VinMonitors.h"
#include "ClockDisplay.h"
//#include "LEDs.h"
#include <Arduino.h>
#include <avr/wdt.h>


// See Config.h for pin and other configuration

// Global variables

Heartbeat* heartbeat;
DebouncedButton* resetButton;
Mode* mode = NULL;

#ifdef DEBUGTIME
unsigned long lastLoop = 0;
#endif

enum eModes {
    Wait,
    Countdown,
    Game
};
eModes nextMode = Game;

void setNextMode()
{
#ifdef DEBUG
    int freeb4 = freeMemory();
#endif
    bool start = false;
    if (mode) {
        mode->stop();
        delete mode;
        mode = NULL;
        start = true;
    }
    switch (nextMode) {
    case Wait:
        mode = new WaitMode();
        break;
    case Countdown:
        mode = new CountdownMode();
        break;
    case Game:
        mode = new GameMode();
        break;
    }
    if (start) {
        mode->start();
    }
#ifdef DEBUG
    Serial.print(F("setNextMode() free b4/now: "));
    Serial.print(freeb4);
    Serial.print(F("/"));
    Serial.println(freeMemory());
#endif
}

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

    // Let things settle
    delay(500);

    // enable watchdog reset at 1/4 sec
    //wdt_enable(WDTO_250MS);

    // engage first mode
    setNextMode();

#ifdef DEBUG
    Serial.println(F("setup() E"));
#endif
}

void loop()
{
    // feed the watchdog
    //wdt_reset();

#ifdef DEBUGTIME
    Serial.print(F("looptime="));
    Serial.println(millis() - lastLoop);
    lastLoop = millis();
#endif

#ifdef DEBUGMEM
    Serial.print(F("free="));
    Serial.println(freeMemory());
#endif

    // give a time slice to various peripheral functions
    heartbeat->update();
    resetButton->update();
    pedalVoltage1.update();
    pedalVoltage2.update();
    ArduinoVoltage.update();

    // detect button presses and behave appropriately
    if (resetButton->isPressed()) {
#ifdef DEBUG
        Serial.println(F("BUTTON: starting countdown"));
#endif
        nextMode = Countdown;
        setNextMode();
    }

    mode->update();

    if (mode->isFinished()) {
        if (nextMode == Countdown) {
            nextMode = Game;
        } else {
            nextMode = Wait;
        }
        setNextMode();
    }
}

