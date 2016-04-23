#include <wiringPi.h>

#include "doorman.hpp"
#include "config.h"

static bool hasInitialized(false);

static int writePin(-1);
static int readPin(-1);

void doorman::setupRead(int pin) {
  if (!hasInitialized) {
    ::wiringPiSetupGpio();
    hasInitialized = true;
  }

  if (readPin == -1) {
    ::pinMode(pin, INPUT);
    readPin = pin;
  }
};

void doorman::setupWrite(int pin) {
  if (!hasInitialized) {
    ::wiringPiSetupGpio();
    hasInitialized = true;
  }

  if (writePin == -1) {
    ::pinMode(pin, OUTPUT);
    writePin = pin;
  }
};

int doorman::read() {
  return ::digitalRead(readPin);
};

void doorman::write(int value) {
  return ::digitalWrite(writePin, value);
};

void doorman::sleep() {
  ::delayMicroseconds(FREQUENCY);
};
