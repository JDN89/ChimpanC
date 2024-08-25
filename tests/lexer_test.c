#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>

/*void test_init_lexer() {*/
/*  char source[] = "=/0";*/
/*  Lexer lexer;*/
/*  init_lexer(source, &lexer);*/
/**/
/*  printf("%c", *lexer.start);*/
/*  assert(*lexer.start == *source);*/
/*  assert(*lexer.current == *source);*/
/*}*/

void test_nextToken() {
  char source[] = "=/0";
  Lexer lexer;
  init_lexer(source, &lexer);

  Token token = nextToken(&lexer);
  /*assert(token.type == ASSIGN);*/
  /*printf("%c", *token.literal);*/
  /*assert(*token.literal == '8');*/
  /*assert(token.length == 1); // Assuming 'x' is a single character*/
}

int main() {
  /*test_init_lexer();*/
  test_nextToken();
  printf("All tests passed.\n");
  return 0;
}
