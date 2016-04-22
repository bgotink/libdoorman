#ifndef _DOORMAN_H
#define _DOORMAN_H 1

namespace doorman {

void setupRead();
void setupWrite();

int read();
void write(int);

void sleep();

};

#endif // _DOORMAN_H
