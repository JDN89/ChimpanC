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
  case TOKEN_GT:
    return ">";
  case TOKEN_LT:
    return "<";
  case TOKEN_ASTERISK:
    return "*";
  case TOKEN_SLASH:
    return "/";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_BANG:
    return "!";
  case TOKEN_FALSE:
    return "false";
  case TOKEN_IF:
    return "if";
  case TOKEN_ELSE:
    return "else";
  case TOKEN_TRUE:
    return "true";
  default:
    return "TOKEN_UNKNOWN";
  }
}

// Helper function to check token equality with error reporting
void check_token(int position, Token token, TokenType expected_type,
                 const char *expected_literal, int expected_length) {
  if (token.type != expected_type) {
    fprintf(stderr,
            "Error at token position %d: Expected token type %s, but got %s\n",
            position, TokenTypeToString(expected_type),
            TokenTypeToString(token.type));
    assert(token.type == expected_type);
  }

  if (strncmp(token.literal, expected_literal, expected_length) != 0) {
    fprintf(stderr,
            "Error at %d: Expected token literal '%s', but got '%.*s'\n",
            position, expected_literal, token.length, token.literal);
    assert(strncmp(token.literal, expected_literal, expected_length) == 0);
  }

  if (token.length != expected_length) {
    fprintf(stderr, "Error at %d: Expected token length %d, but got %d\n",
            position, expected_length, token.length);
    assert(token.length == expected_length);
  }
}
