#include <iostream>
#include <chrono>
#include <wiringPi.h>

#include <boost/lockfree/spsc_queue.hpp>

#include "parser.hpp"

#define PIN_READ 27
#define PARSER_WAIT 200

#define ONE_BIT_LENGTH 200
#define MIN_SIGNAL_LENGTH (ONE_BIT_LENGTH / 2)

struct Interrupt {
  std::chrono::high_resolution_clock::time_point timestamp;
  unsigned long long int duration;
  unsigned int value;
};

typedef Interrupt *interrupt_t;
typedef boost::lockfree::spsc_queue<interrupt_t> interrupt_list_t;

/*
 * interrupt_list_t is threadsafe as long as only one thread reads and only one
 * thread writes.
 */

// interrupt_list_t where our interrupt thread writes and main thread reads
interrupt_list_t interrupts;
// interrupt_list_t where our interrupt thread reads and main thread writes
interrupt_list_t empty;

void handleInterrupt(void) {
  interrupt_t interrupt;

  if (empty.read_available()) {
    interrupt = empty.front();
    empty.pop();
  } else {
    interrupt = new Interrupt();
  }

  interrupt->value = (unsigned int) ::digitalRead(PIN_READ);
  interrupt->timestamp = std::chrono::high_resolution_clock::now();

  if (!interrupts.push(interrupt)) {
    delete interrupt;
  }
}

void parse(void) {
  parser parser;
  interrupt_t current, previous;

  previous = new Interrupt();
  previous->value = 2; // doesn't exist
  previous->timestamp = std::chrono::high_resolution_clock::now();
  previous->duration = 0;

  while (true) {
    while (interrupts.read_available()) {
      interrupt = interrupts.front();
      interrupts.pop();

      interrupt->duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        interrupt->timestamp - previous->timestamp
      );

      if (interrupt->duration < MIN_SIGNAL_LENGTH
          || interrupt->value == previous->value) {
        previous->duration += interrupt->duration;

        empty.push(interrupt);
        continue;
      }

      unsigned int count = (unsigned int) round((1.0f * interrupt->duration) / ONE_BIT_LENGTH);

      if (count > 5) {
        count = 5;
      }

      while (count-- > 0) {
        if (parser.consume(interrupt->value)) {
          if (parser.is_ready()) {
            std::cout << parser.bits << std::endl;
            parser.reset();
          }
        } else {
          parser.reset();
        }
      }
    }

    ::delayMicroseconds(PARSER_WAIT);
  }
}
