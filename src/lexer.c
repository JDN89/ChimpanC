#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

  token.literal = lexer->start;
  return token;
}

static void skipWhitespace(Lexer *lexer) {
  for (;;) {
    char c = peek(lexer);
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance(lexer);
      break;
    default:
      return;
    }
  }
}

// NOTE: in case of let is lexer.start 'l' and lexer .current points now at 'e'
static TokenType lookUpIdentifier(Lexer *lexer, char *word, int length,
                                  TokenType type) {
  if (lexer->current - lexer->start == length + 1) {
    bool isKeyWord = memcmp(word, lexer->start, length) == 0;
    if (isKeyWord) {
      return type;
    }
  }
  return IDENTIFIER;
}

static Token makeErrorToken(const char *message) {
  Token token;
  token.type = TOKEN_ILLEGAL;
  token.literal = message;
  token.length = strlen(message);
  return token;
}

Token nextToken(Lexer *lexer) {
  skipWhitespace(lexer);

  lexer->start = lexer->current;

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

    // Scan Identifiers and Keywords
  default:
    if (isChar(c)) {
      // how to stop

      while (isChar(advance(lexer))) {

        if (!isChar(peek(lexer)))
          switch (lexer->start[0]) {
          case 'l':
            return makeToken(lookUpIdentifier(lexer, "et", 2, LET), lexer);
          case 'f':
            return makeToken(lookUpIdentifier(lexer, "un", 2, FUNCTION), lexer);
          default:
            return makeToken(IDENTIFIER, lexer);
          }
      }
    }
  }

  return makeErrorToken("Illegal string in source");
}
