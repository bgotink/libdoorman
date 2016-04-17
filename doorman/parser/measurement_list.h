#ifndef _MEASUREMENT_LIST_H
#define _MEASUREMENT_LIST_H

#include "measurement.h"

namespace doorman {
namespace parser {

struct measurement_list {
  measurement_list(int size = 0);
  ~measurement_list();

  measurement *create(measurement_value_t value = MEASUREMENT_ZERO, int length = 0);
  void destroy(measurement *);
  void push(measurement *);

  measurement *first;
  measurement *last;

private:
  measurement *empty_first;
  measurement *empty_last;
};

}
}
#endif // _MEASUREMENT_LIST_H
