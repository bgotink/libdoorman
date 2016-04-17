#ifndef __parser_h
#define __parser_h

namespace doorman
{
  void init();

  struct state_t;

  class parser_t {
    state_t *state;

  public:
    unsigned int bits[12];
    unsigned int length;

    parser_t();

    void reset();
    bool consume(unsigned int);
    bool is_ready() const;
  };
}

#endif
