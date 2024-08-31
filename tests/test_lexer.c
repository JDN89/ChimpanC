#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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

// Helper function to check token equality
void check_token(Token token, TokenType expected_type,
                 const char *expected_literal, int expected_length) {
  assert(token.type == expected_type);
  assert(strncmp(token.literal, expected_literal, expected_length) == 0);
  assert(token.length == expected_length);
}

void test_init_lexer() {
  char source[] = "=/0";
  Lexer lexer;
  init_lexer(source, &lexer);

  assert(*lexer.start == *source);
  assert(*lexer.current == *source);
  printf("test_lexer: test_init_lexer passed \n");
}

void test_single_char_tokens() {
    char source[] = "=+(){},";
    Lexer lexer;
    init_lexer(source, &lexer);

    TokenType expected_types[] = {TOKEN_ASSIGN, TOKEN_PLUS, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RBRACE, TOKEN_COMMA};
    const char *expected_literals[] = {"=", "+", "(", ")", "{", "}", ","};
    int num_tokens = sizeof(expected_types) / sizeof(TokenType);

    for (int i = 0; i < num_tokens; i++) {
        Token token = nextToken(&lexer);
        check_token(token, expected_types[i], expected_literals[i], 1);
        lexer.start = lexer.current;
    }

    printf("test_single_char_tokens passed \n");
}

void test_identifier() {
    char source[] = "aAzZbB, qwerty";
    Lexer lexer;
    init_lexer(source, &lexer);

    TokenType expected_types[] = {TOKEN_IDENTIFIER, TOKEN_COMMA, TOKEN_IDENTIFIER};
    const char *expected_literals[] = {"aAzZbB", ",", "qwerty"};
    int num_tokens = sizeof(expected_types) / sizeof(TokenType);

    for (int i = 0; i < num_tokens; i++) {
        Token token = nextToken(&lexer);
        check_token(token, expected_types[i], expected_literals[i], strlen(expected_literals[i]));
        lexer.start = lexer.current;
    }

    printf("test_identifier passed \n");
}

void test_keyword() {
    char source[] = "fun let";
    Lexer lexer;
    init_lexer(source, &lexer);

    TokenType expected_types[] = {TOKEN_FUNCTION, TOKEN_LET};
    const char *expected_literals[] = {"fun", "let"};
    int num_tokens = sizeof(expected_types) / sizeof(TokenType);

    for (int i = 0; i < num_tokens; i++) {
        Token token = nextToken(&lexer);
        check_token(token, expected_types[i], expected_literals[i], strlen(expected_literals[i]));
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
