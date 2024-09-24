#include "parser.h"
#include "lexer.h"
#include <stdio.h>

// TODO: convert program to a dynamic array of AST nodes
// simplify the code a lot!! espicialy the creation of statments seems
// convoluted


void getToken(Parser *p) {
  p->curToken = p->peekToken;
  p->peekToken = nextToken(p->l);
}

Parser newParser(Lexer *l) {
  Parser p;
  p.l = l;

  // NOTE:Read two tokens, so curToken and peekToken are both set
  getToken(&p);
  getToken(&p);
  return p;
};

LetStmt *parseLetStatement(Parser *p) {
  LetStmt *stmt;
  if (p->peekToken.type == TOKEN_IDENTIFIER) {

    *stmt = createLetStmt(p->peekToken);
  } else {
    printf("Expected LET token but got: %d", p->curToken.type);
  }

  while (p->curToken.type != TOKEN_SEMICOLON) {
    getToken(p);
  }
  return stmt;
}

Stmt *parseStatement(Parser *p) {
  Stmt *stmt;
  switch (p->curToken.type) {
  case TOKEN_LET: {
    return createStmt(LET_STATEMENT, parseLetStatement(p));
  }
  default:
    NULL;
  }
  return NULL;
}

Program parseProgram(Parser *p) {
  Program prog = createProgram();
  while (p->curToken.type != EOF) {
    parseStatement(p);
  }

  return prog;
}
