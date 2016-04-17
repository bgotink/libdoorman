#include "parser.h"

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <stdexcept>

using namespace std;

namespace doorman { namespace parser {

// measurement

measurement::measurement(int value, int length) : value(value), length(length), next(NULL) {
  // NOP
};

~measurement::measurement() {
  if (hasNext()) {
    delete next;
    next = NULL;
  }
};

void measurement::clear() {
  value = length = 0;
}

}};

// measurement_list

measurement_list::measurement_list(int size) : first(NULL), last(NULL), empty_first(NULL), empty_last(NULL) {
  if (size > 0) {
    empty_first = new measurement();

    while (size > 1) {
      measurement *current = new measurement();
      current->setNext(empty_first);
      empty_first = current;

      size--;
    }
  }
};

measurement_list::~measurement_list() {
  if (first != NULL) {
    delete first;
    first = NULL;
    last = NULL;
  }

  if (empty_first != NULL) {
    delete empty_first;
    empty_first = NULL;
    empty_last = NULL;
  }
};

measurement *measurement_list::create() {
  measurement *result = NULL;

  if (empty_first != NULL) {
    result = empty_first;

    if (result->hasNext()) {
      empty_first = result->getNext();
    } else {
      empty_first = empty_last = NULL;
    }

    result->setNext(NULL);
  } else {
    result = new measurement();
  }

  return result;
};

void measurement_list::destroy(measurement *meas) {
  if (meas == NULL) {
    return;
  }

  measurement *last = meas;

  while (last->hasNext()) {
    last->clear();
    last = last->getNext();
  }

  empty_last->setNext(meas);
  empty_last = last;
}

// locking

static mutext parser_mutex;
static condition_variable parser_condition;
static atomic_bool parser_locked = false;

static void lock() {
  unique_lock<mutex> parser_lock(parser_mutex);

  if (parser_locked) {
    throw std::runtime_error("impossible scenario: parser already locked");
  }

  parser_locked = true;
  while (parser_locked) {
    parser_condition.wait(parser_lock);
  }
}

void unlock() {
  if (!parser_locked) {
    return;
  }

  parser_locked = false;
  parser_condition.notify_all();
}

// initialisation

void init() {
  // TODO
}

// parsing

static measurement_list list_of_measurements;

void parse() {
  
}
