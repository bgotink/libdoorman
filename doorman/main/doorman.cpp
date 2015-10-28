#include "doorman.h"
#include <wiringPi.h>

#include <string>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <cstdlib>

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

int do_write(char *channel_str, char *sound_str) {
  static int delay = 200;
  int channel = atoi(channel_str);
  int sound = atoi(sound_str);
  static int channel_map[] = {0b1111,0b0011,0b1011,0b0000,0b1000,0b0100,0b1100,0b0101,0b1101,0b0001,0b1001,0b0010,0b1010,0b0110,0b1110,0b0111};
  if(channel > channel_map.size() || channel < 1){
    cerr << "Channel not existing" << newline
    return 1
  }
  int channel_bit = channel_map[channel - 1];

  static int sound_map[] = {0b11110111,0b11111011,0b11111101};
  if(sound > sound_map.size() || channel < 1){
    cerr << "Channel not existing" << newline
    return 1
  }
  int sound_bit = sound_map[sound - 1];


  for(int i = 0; i < 10; i++){
    for(int j = 3; j >= 0; j--){
       doorman::write(1);
       delayMicroseconds(delay);
       doorman::write(0);
       delayMicroseconds(delay);
       doorman::write(channel_bit & (1 << j));
       delayMicroseconds(delay);
    }
    for(int j=7; j >= 0; j--){
       doorman::write(1);
       delayMicroseconds(delay);
       doorman::write(0);
       delayMicroseconds(delay);
       doorman::write(sound_bit & (1 << j));
       delayMicroseconds(delay);
    }
    doorman::write(1);
    delayMicroseconds(delay);
    doorman::write(0);
    delayMicroseconds(delay*30);
  }
return 0;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    fprintf(stderr, "Invalid number of arguments: %d, at least 2 \nUsage: %s <mode>\n", argc - 1, argv[0]);
    return 1;
  }

  char *command = argv[2];
  char *file = (argc == 3) ? argv[3] : 0;

  doorman::setup();

  if (command_read == argv[1]) {
    return do_read(file, false);
  } else if (command_readHuman == argv[1]) {
    return do_read(file, true);
  } else if (command_write == argv[1] && argc == 4) {
    return do_write(argv[2], argv[3]);
  } else {
    fprintf(stderr, "Invalid mode \"%s\", valid modes are \"read\", \"read-human\" and \"write <channel> <sound>\"\n", argv[1]);
  }
}
