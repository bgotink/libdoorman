#include "measurement_list.h"

namespace doorman {
namespace parser {

measurement_list::measurement_list(int size) {
  first = last = nullptr;

  if (size < 1) {
    empty_first = empty_last = nullptr;
    return;
  }

  empty_first = empty_last = new measurement();

  measurement *tmp;
  while (size > 1) {
    tmp = new measurement();

    tmp.setNext(empty_first);
    empty_first = tmp;

    size--;
  }
}

measurement_list::~measurement_list() {
  if (first) {
    delete first;
    first = nullptr;
  }

  if (empty_first) {
    delete empty_first;
    empty_first = nullptr;
  }
}

measurement*
measurement_list::create(measurement_value_t value, int length) {
  if (empty_first) {
    measurement *result = empty_first;
    empty_first = empty_first->getNext();

    if (!empty_first) {
      empty_last = nullptr;
    }

    result->clear();
    result->setValue(value);
    result->setLength(length);

    return result;
  }

  return new measurement();
}

void
measurement_list::destroy(measurement *m) {
  if (empty_last) {
    empty_last->setNext(m);
  } else {
    empty_first = m;
  }

  while (m->hasNext()) {
    m = m->getNext();
  }
  empty_last = m;
}

void
measurement_list::push(measurement *m) {
  if (last) {
    last->setNext(m);
  } else {
    first = m;
  }

  while (m->hasNext()) {
    m = m->getNext();
  }
  last = m;
}

}
}
