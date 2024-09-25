#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

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
  LetStmt *stmt = malloc(sizeof(LetStmt));
  if (p->peekToken.type == TOKEN_IDENTIFIER) {

    createLetStmt(p->peekToken);
  } else {
    printf("Expected LET token but got: %d", p->curToken.type);
  }

  while (p->curToken.type != TOKEN_SEMICOLON) {
    getToken(p);
  }
  return stmt;
}

Stmt *parseStatement(Parser *p) {
  // Dynamically allocate memory for Stmt
  Stmt *stmt = malloc(sizeof(Stmt));
  if (stmt == NULL) {
    fprintf(stderr, "Memory allocation failed for Stmt\n");
    exit(EXIT_FAILURE);
  }

  switch (p->peekToken.type) {
  case TOKEN_LET: {
    LetStmt *letStmt = parseLetStatement(p);
    if (letStmt != NULL) {
      stmt->type = LET_STATEMENT; // Set the type of the statement
      stmt->as.letStmt = letStmt; // Assign the parsed let statement
    } else {
      free(stmt);  // Free the statement if parsing failed
      return NULL; // Return NULL if parsing failed
    }
    break;
  }
  default:
    printf("Unexpected token: %d\n", p->curToken.type);
    free(stmt);  // Free memory if the statement could not be parsed
    return NULL; // Return NULL for unexpected token
  }

  return stmt; // Return the dynamically allocated statement
}

Program parseProgram(Parser *p) {
  Program prog = createProgram();
  // NOTE: we stop looping in parseLetStmt and probably everywhere when curToken
  // = ';'
  while (p->peekToken.type != TOKEN_EOF) {
    Stmt *stmt = parseStatement(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
