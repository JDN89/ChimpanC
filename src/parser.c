#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: convert program to a dynamic array of AST nodes
void getToken(Parser *p) {
  p->ct = p->pt;
  p->pt = nextToken(p->l);
}

Parser newParser(Lexer *l) {
  Parser p;
  p.l = l;

  // NOTE:Read two tokens, so curToken and peekToken are both set
  getToken(&p);
  getToken(&p);
  p.errorCount = 0;
  return p;
};

// TODO: createUnwindErrors

void peekError(Parser *p, char *expected, char *got) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "Expected token %s, but got %s \n", expected, got);
  p->errorCount++;
  p->errors[p->errorCount - 1] = msg;
}

// TODO: Now we immediatley exit after calling this function -> better way to do
// this? Can't we just repost and keep parsing?
void freeParserErrors(Parser *p) {
  for (int i = 0; i < +p->errorCount; i++) {
    printf("%s", p->errors[i]);
    free(p->errors[i]);
    p->errors[i] = NULL;
  }
}

bool expectPeekToken(Parser *p, TokenType ttype) {
  if (p->pt.type == ttype) {
    getToken(p);
    return true;
  } else
    peekError(p, tokenTypeToString(ttype),
              tokenTypeToString(p->pt.type));
  return false;
}

LetStmt *parseLetStatement(Parser *p) {
  LetStmt *letStmt = malloc(sizeof(LetStmt));

  if (!expectPeekToken(p, TOKEN_IDENTIFIER)) {
    freeParserErrors(p);
    exit(EXIT_FAILURE);
  }

  letStmt->identifier = makeString(p->pt.literal, p->pt.length);

  if (!expectPeekToken(p, TOKEN_ASSIGN)) {
    freeParserErrors(p);
    exit(EXIT_FAILURE);
  }

  while (p->ct.type != TOKEN_SEMICOLON) {
    getToken(p);
  }
  return letStmt;
}

ReturnStatement *parseReturnStatement(Parser *p) {
  ReturnStatement *returnStatement = malloc(sizeof(ReturnStatement));
  if (returnStatement == NULL) {

    fprintf(stderr, "Memory allocation failed for ReturnStatement\n");
  }
  getToken(p);
  returnStatement->type = TOKEN_RETURN;
  while (p->ct.type != TOKEN_SEMICOLON) {
    getToken(p);
  }
  return returnStatement;
}

Stmt *parseStatement(Parser *p) {

  Stmt *stmt = malloc(sizeof(Stmt));

  if (stmt == NULL) {
    fprintf(stderr, "Memory allocation failed for Stmt\n");
    exit(EXIT_FAILURE);
  }

  switch (p->ct.type) {

  case TOKEN_LET: {
    LetStmt *letStmt = parseLetStatement(p);
    if (letStmt != NULL) {
      stmt->type = LET_STATEMENT;
      stmt->as.letStmt = letStmt;
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }
  case TOKEN_RETURN: {
    ReturnStatement *returnStatement = parseReturnStatement(p);
    if (returnStatement != NULL) {
      stmt->type = RETURN_STATEMENT;
      stmt->as.returnStmt = returnStatement;
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }
  default:
    printf("Unexpected token: %s in: %s\n", tokenTypeToString(p->ct.type),
           p->ct.literal);
    free(stmt);
    exit(EXIT_FAILURE);
  }

  return stmt;
}

Program parseProgram(Parser *p) {
  Program prog = createProgram();
  // NOTE: we stop looping in parseLetStmt and probably everywhere when curToken
  // = ';'
  while (p->pt.type != TOKEN_EOF) {
    Stmt *stmt = parseStatement(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
