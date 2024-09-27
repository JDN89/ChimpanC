#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: convert program to a dynamic array of AST nodes
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

bool expectPeekToken(Parser *p, TokenType token) {
  if (p->peekToken.type == token) {
    getToken(p);
    return true;
  } else
    return false;
}

LetStmt *parseLetStatement(Parser *p) {
  LetStmt *letStmt = malloc(sizeof(LetStmt));

  if (!expectPeekToken(p, TOKEN_IDENTIFIER)) {
    fprintf(stderr, "Expected TOKEN_IDENTIFIER \n");
    exit(EXIT_FAILURE);
  }

  letStmt->identifier = makeString(p->peekToken.literal, p->peekToken.length);

  if (!expectPeekToken(p, TOKEN_ASSIGN)) {
    fprintf(stderr, "Expected TOKEN_ASSIGN \n");
    exit(EXIT_FAILURE);
  }

  while (p->curToken.type != TOKEN_SEMICOLON) {
    getToken(p);
  }
  return letStmt;
}

Stmt *parseStatement(Parser *p) {
  // Dynamically allocate memory for Stmt
  Stmt *stmt = malloc(sizeof(Stmt));
  if (stmt == NULL) {
    fprintf(stderr, "Memory allocation failed for Stmt\n");
    exit(EXIT_FAILURE);
  }
  switch (p->curToken.type) {
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
    printf("Unexpected token: %s\n", p->curToken.literal);
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
