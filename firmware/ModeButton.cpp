#pragma once

#include "DebouncedButton.h"
#include "DualButton.h"
#include "Config.h"

DebouncedButton ModeButtonWired(MODE_BUTTON_PIN, true);
DebouncedButton ModeButtonRadio(MODE_RADIO_PIN, false);
DualButton ModeButton(&ModeButtonWired, &ModeButtonRadio);
