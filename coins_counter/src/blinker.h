#pragma once
#include <Arduino.h>

typedef enum { blinking, stop } BlinkerState;

class Blinker {
 public:
  Blinker() {}

  int state = LOW;

  void setBlink(unsigned long highPeriod, unsigned long lowPeriod,
                unsigned long count) {
    _setTime = millis();
    _highPeriod = highPeriod;
    _lowPeriod = lowPeriod;
    _count = count;
    _state = BlinkerState::blinking;
  }
  void update() {
    unsigned long currentTime = millis() - _setTime;
    if (_state == BlinkerState::blinking) {
      bool isBlinking =
          (_count == 0) ? true
                        : currentTime < ((_highPeriod + _lowPeriod) * _count);
      if (isBlinking) {
        if ((_highPeriod + _lowPeriod) > 0 &&
            currentTime % (_highPeriod + _lowPeriod) < _highPeriod) {
          state = HIGH;
        } else {
          state = LOW;
        }
      } else {
        state = LOW;
        _state = BlinkerState::stop;
      }
    } else if (_state == BlinkerState::stop) {
      // do nothing
    }
  }

 private:
  unsigned long _setTime = 0;
  unsigned long _highPeriod = 500;
  unsigned long _lowPeriod = 500;
  int _count = 0;
  BlinkerState _state = BlinkerState::stop;
};
