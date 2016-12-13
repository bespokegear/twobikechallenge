#pragma once

#include "DebouncedButton.h"
#include "DualButton.h"
#include "Config.h"

DebouncedButton ResetButtonWired(RESET_BUTTON_PIN);
DebouncedButton ResetButtonRadio(RESET_RADIO_PIN, false);
DualButton ResetButton(&ResetButtonWired, &ResetButtonRadio);

