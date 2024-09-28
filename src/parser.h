#ifndef PARSER_H

#define PARSER_H

#include "ast.h"
#include "lexer.h"

#define ERROR_SIZE 10

typedef struct {
  Lexer *l;
  Token ct;
  Token pt;
  char *errors[ERROR_SIZE];
  int errorCount;
} Parser;

typedef struct {
  Token token;
  char *value;
} Identifier;

Parser newParser(Lexer *l);

Program parseProgram(Parser *p);

void getToken(Parser *p);

void freeParserErrors(Parser *p);


#endif
