#include "dynstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String *makeString(const char *source, int length) {

  String *string = malloc(sizeof(String));
  if (string == NULL) {
    fprintf(stderr, "Failed to allocate memory for String struct");
    return NULL;
  }

  char *heapChars = malloc(length + 1);
  if (heapChars == NULL) {
    fprintf(stderr, "Failed to allocate memory for String characters");
    free(string);
    return NULL;
  }

  memcpy(heapChars, source, length);
  heapChars[length] = '\0';
  // NOTE: does not include the null terminated integer
  string->length = length;
  string->chars = heapChars;

  return string;
}

void freeString(String *string) {
  if (string != NULL) {
    free(string->chars);
    free(string);
  }
}
