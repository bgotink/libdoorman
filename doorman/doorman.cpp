#include <wiringPi.h>

#include "doorman.hpp"
#include "config.h"

static bool hasInitializedWP(false);
static bool hasInitializedRead(false);
static bool hasInitializedWrite(false);

void doorman::setupRead() {
  if (!hasInitializedWP) {
    ::wiringPiSetupGpio();
    hasInitializedWP = true;
  }

  if (!hasInitializedRead) {
    ::pinMode(PIN_READ, INPUT);
    hasInitializedRead = true;
  }
};

void doorman::setupWrite() {
  if (!hasInitializedWP) {
    ::wiringPiSetupGpio();
    hasInitializedWP = true;
  }

  if (!hasInitializedWrite) {
    ::pinMode(PIN_WRITE, OUTPUT);
    hasInitializedWrite = true;
  }
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
