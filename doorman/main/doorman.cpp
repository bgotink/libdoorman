#include "doorman.h"

#include <string>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

string command_read("read");
string command_readHuman("read-human");
string command_write("write");

#define COUNT 500000

int content[COUNT];

char newline = '\n';

int do_read(char *file, bool human) {
  cerr << "Listening... ";

  for (int i = 0; i < COUNT; i++) {
    content[i] = doorman::read();
    doorman::sleep();
  }

  cerr << "Done" << newline;

  ostream *output;

  if (file != 0) {
    output = new ofstream(file);
  } else {
    output = &cout;
  }

  cerr << "Writing... ";

  if (human) {
    int count = 0;
    int current, previous = -1;

    for (int i = 0; i < COUNT; i++) {
      current = content[i];

      if (current == previous) {
        count++;
      } else {
        if (previous != -1) {
          count = (int) round(count / 20.f);

          if (count > 20) {
            *output << newline;
          } else {
            for (int j = 0; j < count; j++) {
              *output << previous;
            }
          }
       }

        previous = current;
        count = 1;
      }
    }
  } else {
    for (int i = 0; i < COUNT; i++) {
      *output << content[i];
    }
  }

  cerr << "Done" << newline;

  return 0;
}

int do_write(char *file) {
  return 0;
}

int main(int argc, char ** argv) {
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Invalid number of arguments: %d, expected 2 or 3\nUsage: %s <mode> [file]\n", argc - 1, argv[0]);
    return 1;
  }

  char *command = argv[2];
  char *file = (argc == 3) ? argv[3] : 0;

  doorman::setup();

  if (command_read == argv[1]) {
    return do_read(file, false);
  } else if (command_readHuman == argv[1]) {
    return do_read(file, true);
  } else if (command_write == argv[1]) {
    return do_write(file);
  } else {
    fprintf(stderr, "Invalid mode \"%s\", valid modes are \"read\", \"read-human\" and \"write\"\n", argv[1]);
  }
}
