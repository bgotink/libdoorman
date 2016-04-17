#include "parser.hpp"

namespace doorman {

typedef bool (parser_state_fn *)(unsigned int, parser &);

struct state {
  parser_state_fn fn;
  state *next;
};

static bool state_fn_one(unsigned int bit, parser &parser) {
  return bit == 1;
}

static bool state_fn_zero(unsigned int bit, parser &parser) {
  return bit == 0;
}

static bool state_fn_bit(unsigned int bit, parser &parser) {
  if (parser.length < 12) {
    parser.bits[parser.length] = bit;
    parser.length++;

    return true;
  }

  return false;
}

static bool state_fn_invalid(unsigned int bit, parser &parser) {
  return false;
}

static state one, zero, bit;

one.fn = &state_fn_one;
one.next = &zero;

zero.fn = &state_fn_zero;
zero.next = &bit;

bit.fn = &state_fn_bit;
bit.next = &one;

static state invalid;

invalid.fn = &state_fn_invalid;
invalid.next = &invalid;

parser::parser() : length(0), state(&one) {}

void parser::reset() {
  this->length = 0;
  this->state = &one;
}

bool parser::consume(unsigned int bit) {
  if (this->state->fn(bit, *this)) {
    this->state = this->state->next;
    return true;
  }

  this->state = &invalid;
  return false;
}

}; // end namespace doorman
