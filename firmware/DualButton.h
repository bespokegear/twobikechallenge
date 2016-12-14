#pragma once

#include "DebouncedButton.h"

class DualButton {
public:
    DualButton(DebouncedButton* b1, DebouncedButton* b2);
    void begin(uint8_t threshold=DEBOUNCED_BUTTON_THRESHOLD, uint8_t delay=DEBOUNCED_BUTTON_DELAY);
    void update();
    bool on();
    bool pushed();
    bool tapped();
    bool held(uint16_t ms=DEBOUNCED_BUTTON_HELD_MS);
    bool repeat(uint16_t initialMs=DEBOUNCED_BUTTON_RPT_INITIAL_MS, uint16_t repeatMs=DEBOUNCED_BUTTON_RPT_MS);

private:
    DebouncedButton* _b1;
    DebouncedButton* _b2;
};

