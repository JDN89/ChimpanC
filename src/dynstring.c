#include "dynstring.h"
#include <stdlib.h>
#include <string.h>

String *makeString(char *source, int length) {

  String *string = malloc(sizeof(String));

  char *heapChars = malloc(length + 1);
  memcpy(heapChars, source, length);
  heapChars[length] = '\0';
  string->length = length;
  string->chars = heapChars;

  return string;
}
