#include "TwoColorLed.h"

// static callback function to call the appropriate instance method
void TwoColorLed::led_callback_service(void *ptr) {
    TwoColorLed* led_object = static_cast<TwoColorLed*>(ptr);
    led_object->timerCallback();
}

TwoColorLed::TwoColorLed(const uint8_t driveAPin, const uint8_t driveBPin, const bool activeLow) :
    _driveAPin(driveAPin),
    _driveBPin(driveBPin),
    _activeLow(activeLow),
    _color(0),
    _level(0),
    _onTime(0),
    _offTime(0),
    _on(true)
{
    pinMode(_driveAPin, OUTPUT);
    analogWrite(_driveAPin, 0);
    pinMode(_driveBPin, OUTPUT);
    analogWrite(_driveBPin, 0);
    os_timer_setfn(&_timer, led_callback_service, this);
}

void TwoColorLed::setColor(uint16_t color) {
    _color = color;
    updatePins();
}

void TwoColorLed::setLevel(uint16_t level) {
    _level = level;
    updatePins();
}

void TwoColorLed::setBlink(uint16_t onTime, uint16_t offTime) {
    if (onTime == 0) {
        _onTime = 0;
        _offTime = 0;
        _on = true;
        os_timer_disarm(&_timer);
    } else if (offTime) {
        _onTime = onTime;
        _offTime = offTime;
        _on = false;
        os_timer_disarm(&_timer);
        os_timer_arm(&_timer, _offTime, false);
    } else {
        _onTime = onTime;
        _offTime = onTime;
        _on = false;
        os_timer_disarm(&_timer);
        os_timer_arm(&_timer, _offTime, false);
    }
    updatePins();
}

uint16_t TwoColorLed::readColor() {
    return _color;
}

uint16_t TwoColorLed::readLevel() {
    return _level;
}

void TwoColorLed::updatePins() {
    uint32_t levelA;
    uint32_t levelB;
    if (_on && _level > 0) {
        levelA = ((1023 - _color) * (uint32_t)_level) / 1023;
        levelB = (_color * (uint32_t)_level) / 1023;
    } else {
        levelA = 0;
        levelB = 0;
    }
    if (_activeLow) {
        levelA = 1023 - levelA;
        levelB = 1023 - levelB;
    }
    analogWrite(_driveAPin, levelA);
    analogWrite(_driveBPin, levelB);
}

void TwoColorLed::timerCallback() {
    os_timer_disarm(&_timer);
    if (_on) {
        os_timer_arm(&_timer, _offTime, false);
        _on = false;
    } else {
        os_timer_arm(&_timer, _onTime, false);
        _on = true;
    }
    updatePins();
}
