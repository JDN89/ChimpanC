#include "../src/dynstring.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_make_string() {
  char source[] = "let foobar";
  char target[] = "let";
  size_t length = strlen(target);

  String *string = makeString(source, length);
  printf("length = %zu \n",length);
  printf("length = %d \n",string->length);
  assert(string->length == 4);
  printf("make string test passed \n");
}

int main() { test_make_string(); }
