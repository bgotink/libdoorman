#include <iostream>
#include "../parser.hpp"

using std::cin;
using std::cout;
using std::endl;

#define print_parser \
  for (int __i = 0; __i < parser.length; __i++) {\
    cout << parser.bits[__i];\
  }\
  cout << endl;

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
        print_parser
        parser.reset();
      } else {
        print_parser
      }
    } else {
      cout << "Unexpected bit " << bit << endl;
      parser.reset();
    }
  }

  return 0;
}
