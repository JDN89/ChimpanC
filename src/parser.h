#ifndef PARSER_H

#define PARSER_H

#include "lexer.h"
#include "ast.h"

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

Program parseProgram(Parser *p);

void getToken(Parser *p);

#endif
