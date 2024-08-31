#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>

// HELPER METHOD for printing enum values of TokenType
const char *TokenTypeToString(TokenType type) {
  switch (type) {
  case TOKEN_ILLEGAL:
    return "TOKEN_ILLEGAL";
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_INT:
    return "TOKEN_INT";
  case TOKEN_ASSIGN:
    return "TOKEN_ASSIGN";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_LPAREN:
    return "TOKEN_LPAREN";
  case TOKEN_RPAREN:
    return "TOKEN_RPAREN";
  case TOKEN_LBRACE:
    return "TOKEN_LBRACE";
  case TOKEN_RBRACE:
    return "TOKEN_RBRACE";
  case TOKEN_FUNCTION:
    return "TOKEN_FUNCTION";
  case TOKEN_LET:
    return "TOKEN_LET";
  default:
    return "TOKEN_UNKNOWN";
  }
}

void test_init_lexer() {
  char source[] = "=/0";
  Lexer lexer;
  init_lexer(source, &lexer);

  assert(*lexer.start == *source);
  assert(*lexer.current == *source);
  printf("test_lexer: test_init_lexer passed \n");
}

//------------------------------------------------
void test_single_char_tokens() {
  char source[] = "=+(){},";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {TOKEN_ASSIGN, TOKEN_PLUS,   TOKEN_LPAREN,
                                TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE,
                                TOKEN_COMMA};
  const char *expected_literals[] = {"=", "+", "(", ")", "{", "}", ","};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    assert(token.type == expected_types[i]);
    assert(*token.literal == *expected_literals[i]);
    assert(token.length == 1); // Assuming 'x' is a single character
    lexer.start++;
  }
  printf("test_lexer: single char tokens passed \n");
}

// ====================================
// scan IDENTIFIERS test
// ====================================
void test_identifier() {
  char source[] = "aAzZbB, qwerty";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {TOKEN_IDENTIFIER, TOKEN_COMMA,
                                TOKEN_IDENTIFIER};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    assert(token.type == expected_types[i]);
    // NOTE: check if they point to the same address, becasue we pass pointers
    // to the string and we don't copy the actual value
    assert(token.literal == lexer.start);
    lexer.start = lexer.current;
  }
  printf("test_isChar passed \n");
}

//=======================================
// KEYWORD test
// ======================================
void test_keyword() {
  char source[] = "fun let";
  Lexer lexer;
  init_lexer(source, &lexer);

  // Expected token types and literals
  TokenType expected_types[] = {TOKEN_FUNCTION, TOKEN_LET};
  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {

    Token token = nextToken(&lexer);
    printf("type: %s \n",
           TokenTypeToString(
               token.type)); // Print string representation of token type
    assert(token.type == expected_types[i]);
    // NOTE: check if they point to the same address, becasue we pass pointers
    // to the string and we don't copy the actual value
    assert(token.literal == lexer.start);
    lexer.start = lexer.current;
  }
  printf("test_keyword passed \n");
}

int main() {
  test_init_lexer();
  test_single_char_tokens();
  test_identifier();
  test_keyword();
  printf("All tests passed.\n");
  return 0;
}
