#include "../src/value.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_make_string() {
  char *source[3] = {"hello you.\n Yes you!", "dikke tetten.", "yo4"};
  int lengthOfStringLiteral[] = {5, 9, 3};

  char *expectedLiterals[] = {"hello", "dikke tet", "yo4"};

  int i = 0;
  while (i < 3) {

    Value *val = createStringValue(lengthOfStringLiteral[i], source[i]);
    assert(val->type == VAL_STRING);
    assert(strcmp(expectedLiterals[i], val->as.string->pointer) == 0);
    assert(lengthOfStringLiteral[i] == val->as.string->length);

    freeValue(val);
    i++;
  }

  printf("Create string value - SUCCESS! \n");
}

int main() { test_make_string(); }
