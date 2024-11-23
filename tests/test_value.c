#include "../src/value.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_parse_string_value() {
  char *source[3] = {"hello you.\n Yes you!", "dikke tetten.", "yo4"};
  size_t lengthOfStringLiteral[] = {5, 9, 3};

  char *expectedLiterals[] = {"hello", "dikke tet", "yo4"};

  int i = 0;
  while (i < 3) {

    Value *val = create_string_value(lengthOfStringLiteral[i], source[i]);
    assert(val->type == VAL_STRING);
    assert(strcmp(expectedLiterals[i], val->as.string->pointer) == 0);
    assert(lengthOfStringLiteral[i] == val->as.string->length);

    freeValue(val);
    i++;
  }

  printf("Create string value - PASSED! \n");
}

void test_parse_number_value() {
  char *source[3] = {"56 hello you.", "99999  mofo;", "666.55\n!!"};

  double expectedLiterals[] = {56, 99999, 666.55};

  int i = 0;
  while (i < 3) {

    Value *val = createNumberValue(source[i]);
    assert(val->type == VAL_NUMBER);
    assert(val->as.number == expectedLiterals[i]);

    freeValue(val);
    i++;
  }

  printf("Create number Value - PASSED! \n");
}

int main() {
  test_parse_string_value();
  test_parse_number_value();
  printf("\n");
  return 0;
}
