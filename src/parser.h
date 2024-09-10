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

typedef struct {
  Token token;
  Identifier *name;

} Statement;

typedef struct {
  Statement *statement;
} Program;

Parser newParser(Lexer *l);

Program *parseProgram();
void getToken(Parser *p);

#endif
