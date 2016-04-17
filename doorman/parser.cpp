#include "parser.hpp"

namespace doorman {

typedef bool (* parser_state_fn)(unsigned int, parser_t &);

struct state_t {
  parser_state_fn fn;
  state_t *next;
};

static bool state_fn_one(unsigned int bit, parser_t &parser) {
  return bit == 1;
}

static bool state_fn_zero(unsigned int bit, parser_t &parser) {
  return bit == 0;
}

static bool state_fn_bit(unsigned int bit, parser_t &parser) {
  if (parser.length < 12) {
    parser.bits[parser.length] = bit;
    parser.length++;

    return true;
  }

  return false;
}

static bool state_fn_invalid(unsigned int bit, parser_t &parser) {
  return false;
}

static state_t one, zero, bit;
static state_t invalid;

void init() {
  one.fn = &state_fn_one;
  one.next = &zero;

  zero.fn = &state_fn_zero;
  zero.next = &bit;

  bit.fn = &state_fn_bit;
  bit.next = &one;

  invalid.fn = &state_fn_invalid;
  invalid.next = &invalid;
}

parser_t::parser_t() : length(0), state(&one) {}

void parser_t::reset() {
  this->length = 0;
  this->state = &one;
}

bool parser_t::consume(unsigned int bit) {
  if (this->state->fn(bit, *this)) {
    this->state = this->state->next;
    return true;
  }

  this->state = &invalid;
  return false;
}

bool parser_t::is_ready() const {
  return this->length == 12 && this->state == &bit;
}

}; // end namespace doorman
