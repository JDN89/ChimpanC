#ifndef PARSER_H

#define PARSER_H

#include "ast.h"
#include "lexer.h"

typedef struct {
  Lexer *l;
  Token curToken;
  Token peekToken;
  char *errors;
  int errorCount;
} Parser;

typedef struct {
  Token token;
  char *value;
} Identifier;

Parser newParser(Lexer *l);

Program parseProgram(Parser *p);

void getToken(Parser *p);


#endif
