#ifndef _MEASUREMENT_H
#define _MEASUREMENT_H

namespace doorman {
namespace parser {

typedef enum measurement_value {
  MEASUREMENT_ZERO, MEASUREMENT_ONE
} measurement_value_t;

// forward declaration of friend struct
struct measurement_list;

struct measurement {
  measurement(measurement_value_t value = MEASUREMENT_ZERO, int length = 0);
  ~measurement();

  inline measurement_value_t getValue() const { return value; }
  inline void setValue(measurement_value_t value) { this->value = value; }

  inline int getLength() const { return length; }
  inline void setLength(int length) { this->length = length; }

  inline measurement *getNext() const { return next; }
  inline bool hasNext() const { return next != NULL; }

private:
  measurement_value_t value;
  int length;

  measurement *next;

  void clear();
  inline void setNext(measurement *newNext) { next = newNext; }

  friend struct measurement_list;
};

}
}

#endif // _MEASUREMENT_H
