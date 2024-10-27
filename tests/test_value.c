#include "../src/value.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_make_string() {
  char *source = "hello you";
  int length = 5;
  char *expected_result = "hello";

  Value *val = createStringValue(length, source);
  assert(val->type == VAL_STRING);
  assert(strcmp(expected_result, val->as.string->pointer) == 0);
  assert(length == val->as.string->length);

  freeValue(val);

  printf("Create string value - SUCCESS! \n");
}

int main() { test_make_string(); }
