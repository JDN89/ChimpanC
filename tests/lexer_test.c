#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>

void test_init_lexer() {
  char source[] = "=/0";
  Lexer lexer;
  init_lexer(source, &lexer);

  assert(*lexer.start == *source);
  assert(*lexer.current == *source);
}

void test_nextToken() {
  char source[] = "=+(){},";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {ASSIGN, PLUS,   LPAREN, RPAREN,
                                LBRACE, RBRACE, COMMA};
  const char *expected_literals[] = {"=", "+", "(", ")", "{", "}", ","};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    assert(token.type == expected_types[i]);
    assert(*token.literal == *expected_literals[i]);
    printf("literal: %c \n", *token.literal);
    assert(token.length == 1); // Assuming 'x' is a single character
    lexer.start++;
  }
  printf("test_nextToken passed");
}

int main() {
  test_init_lexer();
  test_nextToken();
  printf("All tests passed.\n");
  return 0;
}
