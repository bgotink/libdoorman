#ifndef _PARSER_PARSER_H
#define  _PARSER_PARSER_H

#include "measurement.h"

namespace doorman {
namespace parser {

void
addMeasurement(measurement_value_t value, int length);

struct signal {
  unsigned int channel;
  unsigned int song;
}

const signal *
parseMeasurements();

}
}

#endif // _PARSER_PARSER_H
