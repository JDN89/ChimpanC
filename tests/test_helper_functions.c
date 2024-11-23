#include "../src/lexer.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

// Helper function to check token equality with error reporting
void check_token(int position, Token token, TokenType expected_type,
                 const char *expected_literal, size_t expected_length) {
  if (token.type != expected_type) {
    fprintf(stderr,
            "Error at token position %d: Expected token type %s, but got %s\n",
            position, token_type_to_string(expected_type),
            token_type_to_string(token.type));
    assert(token.type == expected_type);
  }

  if (strncmp(token.literal, expected_literal, expected_length) != 0) {
    int precision;
    if (token.length > INT_MAX) {
      precision = INT_MAX; // Cap the precision to INT_MAX to avoid overflow
    } else {
      precision = (int)token.length;
    }
    fprintf(stderr,
            "Error at %d: Expected token literal '%s', but got '%.*s'\n",
            position, expected_literal, precision, token.literal);
    assert(strncmp(token.literal, expected_literal, token.length) == 0);
  }

  if (token.length != expected_length) {
    fprintf(stderr, "Error at %d: Expected token length %zx, but got %zx\n",
            position, expected_length, token.length);
    assert(token.length == expected_length);
  }
}
