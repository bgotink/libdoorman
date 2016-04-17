#include <iostream>
#include "../parser.hpp"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char ** argv) {
  doorman::init();
  doorman::parser_t parser;

  unsigned int bit;

  while (!cin.eof()) {
    cin >> bit;

    if (parser.consume(bit)) {
      cout << "Expected bit " << bit << endl;
      if (parser.is_ready()) {
        cout << "Parser ready!" << endl;
        cout << parser.bits[0] << parser.bits[1] << parser.bits[2] << parser.bits[2] <<
            parser.bits[4] << parser.bits[5] << parser.bits[6] << parser.bits[7] << parser.bits[8] << parser.bits[9] << parser.bits[10] << parser.bits[11] << endl;
        parser.reset();
      }
    } else {
      cout << "Unexpected bit " << bit << endl;
      parser.reset();
    }
  }

  return 0;
}
