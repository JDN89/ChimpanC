#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>

// NOTE: HELPER METHODS
const char *TokenTypeToString(TokenType type) {
  switch (type) {
  case TOKEN_ILLEGAL:
    return "TOKEN_ILLEGAL";
  case EOF:
    return "EOF";
  case IDENTIFIER:
    return "IDENTIFIER";
  case INT:
    return "INT";
  case ASSIGN:
    return "ASSIGN";
  case PLUS:
    return "PLUS";
  case COMMA:
    return "COMMA";
  case SEMICOLON:
    return "SEMICOLON";
  case LPAREN:
    return "LPAREN";
  case RPAREN:
    return "RPAREN";
  case LBRACE:
    return "LBRACE";
  case RBRACE:
    return "RBRACE";
  case FUNCTION:
    return "FUNCTION";
  case LET:
    return "LET";
  default:
    return "UNKNOWN";
  }
}

void test_init_lexer() {
  char source[] = "=/0";
  Lexer lexer;
  init_lexer(source, &lexer);

  assert(*lexer.start == *source);
  assert(*lexer.current == *source);
}

//------------------------------------------------
// TODO: find better name for test
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
    assert(token.length == 1); // Assuming 'x' is a single character
    lexer.start++;
  }
  printf("test_nextToken passed \n");
}

// NOTE: scan IDENTIFIERS test
void test_identifier() {
  char source[] = "aAzZbB";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {IDENTIFIER};
  const char *expected_literals[] = {"aAzZbB,"};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    assert(token.type == expected_types[i]);
    assert(*token.literal == *expected_literals[i]);
    assert(token.length == 6); // Assuming 'x' is a single character
    lexer.start++;
  }
  printf("test_isChar passed \n");
}

//=======================================
// NOTE: KEYWORD test
void test_keyword() {
  char source[] = "fun let function";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {FUNCTION};
  const char *expected_literals[] = {"fun"};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    printf("type: %s \n",
           TokenTypeToString(
               token.type)); // Print string representation of token type
    assert(token.type == expected_types[i]);
    assert(*token.literal == *expected_literals[i]);
    assert(token.length == 6); // Assuming 'x' is a single character
    lexer.start++;
  }
  printf("test_keyword passed \n");
}

int main() {
  test_init_lexer();
  test_nextToken();
  test_identifier();
  test_keyword();
  printf("All tests passed.\n");
  return 0;
}
