#ifndef DYNSTRING_H
#define DYNSTRING_H

typedef struct {
  int length;
  char *chars;
} String;

String *makeString(char *source, int length);

#endif
