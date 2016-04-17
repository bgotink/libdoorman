#include "measurement.h"

namespace doorman {
namespace parser {

measurement::measurement(measurement_value_t value, int length)
: value(value), length(length), next(nullptr) {
  // NOP
}

measurement::~measurement() {
  if (next) {
    delete next;
    next = nullptr;
  }
}

void
measurement::clear() {
  value = MEASUREMENT_ZERO;
  length = 0;
  next = nullptr;
}

}
}
