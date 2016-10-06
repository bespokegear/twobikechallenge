#include "Heartbeat.h"
#include <Arduino.h>

Heartbeat::Heartbeat(int pin) :
    _mode(Heartbeat::Normal),
	_pin(pin),
    _pinState(true),
    _lastStateFlip(0),
    _onTime(0),
    _offTime(0)
{
    pinMode(_pin, OUTPUT);
	setMode(_mode);
}

Heartbeat::~Heartbeat()
{
}

Heartbeat::Mode Heartbeat::mode()
{
    return _mode;
}

void Heartbeat::setMode(Mode mode)
{
	_mode = mode;
	switch (_mode) {
	case Normal:
        _onTime = ON_MS_NORMAL;
        _offTime = OFF_MS_NORMAL;
		break;
	case Quick:
        _onTime = ON_MS_QUICK;
        _offTime = OFF_MS_QUICK;
		break;
	case Slow:
        _onTime = ON_MS_SLOW;
        _offTime = OFF_MS_SLOW;
		break;
	case Slower:
        _onTime = ON_MS_SLOWER;
        _offTime = OFF_MS_SLOWER;
		break;
	}
}

void Heartbeat::update()
{
    unsigned long wait = _pinState ? _onTime : _offTime;
    if (millis() - _lastStateFlip >= wait) {
        updatePin(!_pinState);
    }
}

void Heartbeat::updatePin(bool state)
{
    _pinState = state;
    digitalWrite(_pin, _pinState);
    _lastStateFlip = millis();
}


