#include <iostream>
#include <chrono>
#include <wiringPi.h>

#include <boost/lockfree/spsc_queue.hpp>

#include "parser.hpp"
#include "config.h"
#include "doorman.hpp"

#define MIN_SIGNAL_LENGTH (ONE_BIT_LENGTH / 2)

typedef int64_t duration_t;

struct Interrupt {
  std::chrono::high_resolution_clock::time_point timestamp;
  unsigned long long int duration;
  duration_t value;
};

typedef Interrupt *interrupt_t;
typedef boost::lockfree::spsc_queue<interrupt_t, boost::lockfree::capacity<500>> interrupt_list_t;

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

  interrupt->value = (unsigned int) (1 - doorman::read());
  interrupt->timestamp = std::chrono::high_resolution_clock::now();

  if (!interrupts.push(interrupt)) {
    delete interrupt;
  }
}

void parse(void) {
  doorman::parser_t parser;
  interrupt_t current, previous;

  previous = new Interrupt();
  previous->value = 2; // doesn't exist
  previous->timestamp = std::chrono::high_resolution_clock::now();
  previous->duration = 0;

  while (true) {
    while (interrupts.read_available()) {
      current = interrupts.front();
      interrupts.pop();

      current->duration = std::chrono::duration_cast<std::chrono::microseconds>(
        current->timestamp - previous->timestamp
      ).count();

      if (current->duration < MIN_SIGNAL_LENGTH
          || current->value == previous->value) {
        previous->duration += current->duration;

        empty.push(current);
        continue;
      }

      unsigned int count = (unsigned int) round((1.0f * previous->duration) / ONE_BIT_LENGTH);

      if (count > 5) {
        count = 5;
      }

      while (count-- > 0) {
        if (parser.consume(previous->value)) {
          if (parser.is_ready()) {
            std::cout << parser.bits[0] << parser.bits[1] << parser.bits[2] << parser.bits[2] <<
                parser.bits[4] << parser.bits[5] << parser.bits[6] << parser.bits[7] << parser.bits[8] << parser.bits[9] << parser.bits[10] << parser.bits[11] << std::endl;
            fflush(stdout);
            parser.reset();
          }
        } else {
          parser.reset();
        }
      }

      empty.push(previous);
      previous = current;
    }

    ::delayMicroseconds(PARSER_WAIT);
  }
}


int main(int argc, char ** argv) {
  doorman::setup();

  doorman::initParser();

  ::wiringPiISR(PIN_READ, INT_EDGE_BOTH, &handleInterrupt);

  parse();
}
