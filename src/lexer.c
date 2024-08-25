#include "lexer.h"
#include <stdbool.h>
#include <string.h>

Lexer lexer;

Lexer *init_lexer(char *source, Lexer *lexer) {
  lexer->start = source;
  lexer->current = source;
  return lexer;
};

static bool isAtEnd() { return *lexer.current == '\0'; }

static char advance() {
  lexer.current++;
  return lexer.current[-1];
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.length = lexer.current - lexer.start;
  token.literal = lexer.start;
  return token;
}
static Token makeErrorToken(const char *message) {
  Token token;
  token.type = TOKEN_ILLEGAL;
  token.literal = message;
  token.length = strlen(message);
  return token;
}

Token nextToken() {
  lexer.current = lexer.start;
  if (isAtEnd())
    return makeToken(EOF);

  char c = advance();
  switch (c) {
  case '=':
    return makeToken(ASSIGN);
  }
  return makeErrorToken("Illegal string in source");
}
