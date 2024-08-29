#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include<stdio.h>

Lexer *init_lexer(char *source, Lexer *lexer) {
  lexer->start = source;
  lexer->current = source;
  return lexer;
};

static bool isAtEnd(Lexer *lexer) { return *lexer->current == '\0'; }

static char advance(Lexer *lexer) {
  lexer->current++;
  return lexer->current[-1];
}

static bool isChar(char c) {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static char peek(Lexer *lexer) { return lexer->current[0]; }

static Token makeToken(TokenType type, Lexer *lexer) {
  Token token;
  token.type = type;
  token.length = lexer->current - lexer->start;

  printf("%d",token.length);

  token.literal = lexer->start;
  return token;
}
static Token makeErrorToken(const char *message) {
  Token token;
  token.type = TOKEN_ILLEGAL;
  token.literal = message;
  token.length = strlen(message);
  return token;
}

Token nextToken(Lexer *lexer) {
  lexer->current = lexer->start;
  if (isAtEnd(lexer))
    return makeToken(EOF, lexer);

  char c = advance(lexer);
  switch (c) {
  case '=':
    return makeToken(ASSIGN, lexer);
  case ';':
    return makeToken(SEMICOLON, lexer);
  case '(':
    return makeToken(LPAREN, lexer);
  case ')':
    return makeToken(RPAREN, lexer);
  case ',':
    return makeToken(COMMA, lexer);
  case '+':
    return makeToken(PLUS, lexer);
  case '{':
    return makeToken(LBRACE, lexer);
  case '}':
    return makeToken(RBRACE, lexer);
  default:
    if (isChar(c)) {
      // how to stop
      while (isChar(advance(lexer))) {
        if (!isChar(peek(lexer)))
          return makeToken(IDENT, lexer);
      }
    }
  }

  return makeErrorToken("Illegal string in source");
}
