#ifndef PARSER_H

#define PARSER_H

#include "lexer.h"

typedef struct {
  Lexer *l;
  Token curToken;
  Token peekToken;
} Parser;

typedef struct {
  Token token;
  char *value;
} Identifier;


Parser newParser(Lexer *l);

void getToken(Parser *p);

#endif
