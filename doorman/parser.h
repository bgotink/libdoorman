#ifndef _PARSER_H
#define _PARSER_H

namespace doorman {
namespace parser {

struct measurement_list;

struct measurement {
  measurement(int value = 0, int length = 0);
  ~measurement();

  inline int getValue() const { return value; }
  inline int getLength() const { return length; }

  inline measurement *getNext() const { return next; }
  inline bool hasNext() const { return next != NULL; }

private:
  int value;
  int length;

  measurement *next;

  void clear();
  inline void setNext(measurement *newNext) { next = newNext; }

  friend struct measurement_list;
};

struct measurement_list {
  measurement_list(int size = 0);
  ~measurement_list();

  measurement *create();
  void destroy(measurement *);

  measurement *first;
  measurement *last;

private:
  measurement *empty_first;
  measurement *empty_last;
}

void unlock();

void init();
void parse();

}
};

#endif // _PARSER_H
