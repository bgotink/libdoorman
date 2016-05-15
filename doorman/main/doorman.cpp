#include "doorman.hpp"
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

int do_read(int pin, char *file, bool human) {
  doorman::setupRead(pin);

  cerr << "Listening... ";

  for (int i = 0; i < COUNT; i++) {
    content[i] = doorman::read();
    doorman::sleep();
  }

  cerr << "Done" << newline;

  ostream *output;

  if (file != NULL) {
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

int do_write(int pin, char *channel_str, char *sound_str) {
  static int delay = 200;
  int channel = atoi(channel_str);
  int sound = atoi(sound_str);

  static int channel_map[] = {0b1111,0b0011,0b1011,0b0000,0b1000,0b0100,0b1100,0b0101,0b1101,0b0001,0b1001,0b0010,0b1010,0b0110,0b1110,0b0111};
  static int sound_map[] = {0b11110111,0b11111011,0b11111101};

  if (sound < 1 || sound > ((sizeof sound_map) / (sizeof sound_map[0]))) {
    fprintf(stderr, "Invalid sound %d\n", sound);
    return 1;
  }

  if (channel < 1 || sound > ((sizeof channel_map) / (sizeof channel_map[0]))) {
    fprintf(stderr, "Invalid channel %d\n", channel);
    return 1;
  }

  doorman::setupWrite(pin);

  printf("Playing sound %d on channel %d\n", sound, channel);

  int channel_bit = channel_map[channel - 1];

  int sound_bit = sound_map[sound - 1];

  for(int i = 0; i < 100; i++){
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
  if (argc < 4) {
    fprintf(stderr, "Invalid number of arguments: %d, required at least 3 \nUsage: %s <pin> <mode>\n", argc - 1, argv[0]);
    return 1;
  }

  int pin = atoi(argv[1]);

  if (command_read == argv[2]) {
    return do_read(pin, argv[3], false);
  } else if (command_readHuman == argv[2]) {
    return do_read(pin, argv[3], true);
  } else if (command_write == argv[2]) {
    if (argc != 5) {
      fprintf(stderr, "Invalid number of arguments, write requires four arguments: %s <port> write <channel> <sound>\n", argv[0]);
      return 1;
    }
    return do_write(pin, argv[3], argv[4]);
  } else {
    fprintf(stderr, "Invalid mode \"%s\", valid modes are \"read\", \"read-human\" and \"write <channel> <sound>\"\n", argv[2]);
    return 1;
  }
}
