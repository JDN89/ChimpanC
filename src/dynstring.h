#ifndef DYNSTRING_H
#define DYNSTRING_H

typedef struct {
  int length;
  char *chars;
} String;

String *makeString(const char *source, int length);

void freeString(String *sting);

#endif
