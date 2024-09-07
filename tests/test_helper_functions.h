#include "../src/lexer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

const char *TokenTypeToString(TokenType type); 

void check_token(int position, Token token, TokenType expected_type,
                 const char *expected_literal, int expected_length);
