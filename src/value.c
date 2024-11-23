#include "value.h"
#include "utilities.h"

#include <alloca.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void freeString(ObjString *obj);

ObjString *createLiteralSubstring(size_t length, const char *source) {
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

Value *create_string_value(size_t length, const char *source) {
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

Value *createNumberValue(const char *source) {
  Value *value = malloc(sizeof(Value));
  HANDLE_ALLOC_FAILURE(value, "Failed to allocate memory for Value");
  double number = strtod(source, NULL);

  *value = NUMBER(number);
  return value;
}

static void freeString(ObjString *obj) {
  if (obj == NULL) {
    return;
  }
  free(obj->pointer);
  free(obj);
}

void freeValue(Value *val) {

  if (val == NULL) { // Additional runtime check
    return;
  }
  assert(val != NULL);
  switch (val->type) {

  case VAL_NUMBER:
    break;
  case VAL_STRING: {
    freeString(val->as.string);
    break;
  }
  default:
    fprintf(stderr, "Unhandled freeValue type \n");
    break; // Optional, for completeness
  }
  free(val);
}
