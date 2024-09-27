#include "../src/lexer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Helper function to check token equality with error reporting
void check_token(int position, Token token, TokenType expected_type,
                 const char *expected_literal, int expected_length) {
  if (token.type != expected_type) {
    fprintf(stderr,
            "Error at token position %d: Expected token type %s, but got %s\n",
            position, tokenTypeToString(expected_type),
            tokenTypeToString(token.type));
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
