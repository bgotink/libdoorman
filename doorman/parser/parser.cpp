#include "measurement_list.h"
#include "parser.h"

#include <cmath>
#include <list>

using namespace std;

namespace doorman {
namespace parser {

#define NOISE_TRESHOLD 75;
#define ONE_BIT_DURATION 200.f;

static measurement_list measurements(50);

typedef enum parse_result {
  INCOMPLETE, MATCH, INVALID
} parse_result_t;

typedef list<measurement_value_t> values_t;

int
getBitLength(const measurement *const measurement) {
  double bits = measurement->getLength() / ONE_BIT_DURATION;

  bits = nearbyint(bits);

  if (bits < 1) {
    bits = 1;
  }

  return (int) bits;
}

void
addMeasurement(measurement_value_t value, int length) {
  if (length < NOISE_TRESHOLD) {
    if (measurements.last) {
      measurements.last->setLength(
        measurements.last->getLength()
        + length
      );
    }

    return;
  }

  if (measurements.last && measurements.last->getValue() == value) {
    measurements.last->setLength(
      measurements.last->getLength()
      + length
    );

    return;
  }

  measurements.push(measurements.create(value, length));
}

parse_result_t
match10(values_t &values) {
  if (values.size() < 2) {
    return INCOMPLETE;
  }

  measurement_value_t first = values.front(); values.pop_front();
  measurement_value_t second = values.front(); values.pop_front();

  if (first != MEASUREMENT_ONE || second != MEASUREMENT_ZERO) {
    return INVALID;
  }

  return MATCH;
}

const signal *
parseMeasurements() {
  signal *result = new signal();

  values_t values;
  const measurement *m = measurements.first;

  while (m) {
    for (int i = 0; i < getBitLength(m); i++) {
      values.push_back(m->getValue());
    }

    m = m->getNext();
  }
}

}
}
