#include "../src/dynstring.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// TODO: debug this test fails. Why is length nog == 4?
void test_make_string() {
  char source[] = "let foobar";
  char target[] = "let";
  size_t length = strlen(target);

  String *string = makeString(source, length);
  assert(string->length == 3);
  assert(strcmp(target, string->chars) == 0);
  freeString(string);
  printf("Test_make_string passed");
}

int main() { test_make_string(); }
