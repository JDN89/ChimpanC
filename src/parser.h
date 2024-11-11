#ifndef PARSER_H

#define PARSER_H

#include "ast.h"
#include "lexer.h"

#define ERROR_SIZE 256

typedef struct {
  Lexer *l;
  Token ct;
  Token pt;
  char *errors[ERROR_SIZE];
  int errorCount;
} Parser;

Parser new_parser(Lexer *l);

Program parse_program(Parser *p);

void advance(Parser *p);

void freeParserErrors(Parser *p);

uint8_t peek_precedence(TokenType ttype);

#endif
