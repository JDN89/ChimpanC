#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/value.h"

void test_make_string() {
  char *source = "hello you";
  int length = 5;
  char *result ="hello";

  Value *val = createStringValue(length,source);
  assert(val->type == VAL_STRING);
  assert( strcmp(result, val->as.string->pointer) == 0 );
  printf("Create string value - SUCCESS! \n");
}

int main() { test_make_string(); }
