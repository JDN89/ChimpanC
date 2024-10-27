#include "value.h"
#include "utilities.h"

#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ObjString *createLiteralSubstring(int length, char *source) {
  ObjString *obj = malloc(sizeof(ObjString));
  HANDLE_ALLOC_FAILURE(obj, "Failed to allocate ObjString in value.c \n");

  obj->pointer = malloc(length + 1);
  HANDLE_ALLOC_FAILURE(obj->pointer,
                       "Failed to allocate string pointer in value.c\n");

  memcpy(obj->pointer, source, length);
  obj->pointer[length] = '\0';

  obj->length = length;

  return obj;
}

Value *createStringValue(int length, char *source) {
  Value *value = malloc(sizeof(Value));
  HANDLE_ALLOC_FAILURE(value,
                       "Failed to allocate memory for Value in value.c \n");
  ObjString *str = createLiteralSubstring(length, source);
  if (str == NULL) {
    freeString(str);
  }

  value->type = VAL_STRING;
  value->as.string = str;
  return value;
}

Value *createNumberValue(int length, char *source) {
  Value *value = malloc(sizeof(Value));
  HANDLE_ALLOC_FAILURE(value,
                       "Failed to allocate memory for Value in value.c \n");

  char *literal = malloc(sizeof(char) * length +1);
  HANDLE_ALLOC_FAILURE(literal, "Failed allocatig memeory for number literal");

  return value;
}

void freeString(ObjString *obj) {
  if (obj == NULL) {
    return;
  }
  free(obj->pointer);
  free(obj);
}

void freeValue(Value *val) {
  if (val == NULL) {
    return;
  }
  switch (val->type) {

  case VAL_NUMBER:
    free(val);
    break;
  case VAL_STRING: {
    freeString(val->as.string);
    free(val);
  } break;

  default:
    fprintf(stderr, "Unhandled freeValue type \n");
    break; // Optional, for completeness
  }
}
