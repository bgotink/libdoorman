#include <wiringPi.h>

#include "doorman.hpp"
#include "config.h"

void doorman::setup() {
  ::wiringPiSetupGpio();

  ::pinMode(PIN_READ, INPUT);
  ::pinMode(PIN_WRITE, OUTPUT);
};

int doorman::read() {
  return ::digitalRead(PIN_READ);
};

void doorman::write(int value) {
  return ::digitalWrite(PIN_WRITE, value);
};

void doorman::sleep() {
  ::delayMicroseconds(FREQUENCY);
};
