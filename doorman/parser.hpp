#ifndef __parser_h
#define __parser_h

namespace doorman
{
  struct state;

  class parser {
    state *state;

  public:
    unsigned int bits[12];
    unsigned int length;

    parser();

    void reset();
    bool consume(unsigned int);
    inline bool is_ready() const {
      return this->length === 12;
    }
  };
}

#endif
