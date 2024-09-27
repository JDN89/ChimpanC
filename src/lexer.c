#include "lexer.h"
#include <stdbool.h>
#include <string.h>

// HELPER METHOD for printing enum values of TokenType
char *tokenTypeToString(TokenType type) {
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

Lexer init_lexer(char *source) {
  Lexer l;
  l.start = source;
  l.current = source;
  return l;
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
        return makeToken(matchKeywordOrIdentifier(lexer, "let", 3, TOKEN_LET),
                         lexer);
        // TODO: expand with false
      case 'f':
        switch (lexer->start[1]) {
        case 'n':
          return makeToken(
              matchKeywordOrIdentifier(lexer, "fn", 2, TOKEN_FUNCTION), lexer);
        case 'a':
          return makeToken(
              matchKeywordOrIdentifier(lexer, "false", 5, TOKEN_FALSE), lexer);
        default:
          return makeToken(TOKEN_IDENTIFIER, lexer);
        }

      case 't':
        return makeToken(matchKeywordOrIdentifier(lexer, "true", 4, TOKEN_TRUE),
                         lexer);
      case 'e':
        return makeToken(matchKeywordOrIdentifier(lexer, "else", 4, TOKEN_ELSE),
                         lexer);
      case 'i':
        return makeToken(matchKeywordOrIdentifier(lexer, "if", 2, TOKEN_IF),
                         lexer);
      case 'r':
        return makeToken(
            matchKeywordOrIdentifier(lexer, "return", 6, TOKEN_RETURN), lexer);

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
