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

// TODO: add fractional support
static bool isDigit(char c) { return ('0' <= c && c <= '9'); }

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
    case '\n':
      advance(lexer);
      break;
    default:
      return;
    }
  }
}

// NOTE: in case of let is lexer.start 'l' and lexer .current points now at 'e'
static TokenType matchKeywordOrIdentifier(Lexer *lexer, char *word, int length,
                           TokenType type) {
  if (lexer->current - lexer->start == length) {
    bool isKeyWord = memcmp(word, lexer->start, length) == 0;
    if (isKeyWord) {
      return type;
    }
  }
  return TOKEN_IDENTIFIER;
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
    return makeToken(TOKEN_EOF, lexer);

  char c = advance(lexer);

  switch (c) {
  case '=':
    if (peek(lexer) == '=') {
      advance(lexer);
      return makeToken(TOKEN_EQ, lexer);
    } else {
      return makeToken(TOKEN_ASSIGN, lexer);
    }
  case '+':
    return makeToken(TOKEN_PLUS, lexer);
  case '-':
    return makeToken(TOKEN_MINUS, lexer);
  case '!':
    if (peek(lexer) == '=') {
      advance(lexer);
      return makeToken(TOKEN_NOT_EQ, lexer);
    } else {
      return makeToken(TOKEN_BANG, lexer);
    }
  case '/':
    return makeToken(TOKEN_SLASH, lexer);
  case '*':
    return makeToken(TOKEN_ASTERISK, lexer);
  case '<':
    return makeToken(TOKEN_LT, lexer);
  case '>':
    return makeToken(TOKEN_GT, lexer);
  case ';':
    return makeToken(TOKEN_SEMICOLON, lexer);
  case ',':
    return makeToken(TOKEN_COMMA, lexer);
  case '(':
    return makeToken(TOKEN_LPAREN, lexer);
  case ')':
    return makeToken(TOKEN_RPAREN, lexer);
  case '{':
    return makeToken(TOKEN_LBRACE, lexer);
  case '}':
    return makeToken(TOKEN_RBRACE, lexer);

    // Scan Identifiers and Keywords
  default:
    if (isChar(c)) {
      // how to stop

      while (isChar(peek(lexer))) {
        advance(lexer);
      }

      switch (lexer->start[0]) {
      case 'l':
        return makeToken(matchKeywordOrIdentifier(lexer, "let", 3, TOKEN_LET), lexer);
        // TODO: expand with false
      case 'f':
        switch (lexer->start[1]) {
        case 'n':
          return makeToken(matchKeywordOrIdentifier(lexer, "fn", 2, TOKEN_FUNCTION), lexer);
        case 'a':
          return makeToken(matchKeywordOrIdentifier(lexer, "false", 5, TOKEN_FALSE), lexer);
        default:
          return makeToken(TOKEN_IDENTIFIER, lexer);
        }

      case 't':
        return makeToken(matchKeywordOrIdentifier(lexer, "true", 4, TOKEN_TRUE), lexer);
      case 'e':
        return makeToken(matchKeywordOrIdentifier(lexer, "else", 4, TOKEN_ELSE), lexer);
      case 'i':
        return makeToken(matchKeywordOrIdentifier(lexer, "if", 2, TOKEN_IF), lexer);
      case 'r':
        return makeToken(matchKeywordOrIdentifier(lexer, "return", 6, TOKEN_RETURN), lexer);

      default:
        return makeToken(TOKEN_IDENTIFIER, lexer);
      }
    } else if (isDigit(c)) {
      while (isDigit(peek(lexer))) {
        advance(lexer);
      }
      return makeToken(TOKEN_INT, lexer);
    }

    else {
      return makeToken(TOKEN_ILLEGAL, lexer);
    }
  }

  return makeErrorToken("Illegal string in source");
}
