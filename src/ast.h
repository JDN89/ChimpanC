#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdio.h>

// NOTE: bothe epxression and STmt have to return the literal value of the token

#define IS_LET_STMT(stmt) ((stmt)->type == LET_STATEMENT)
#define AS_LET_STMT(stmt) ((stmt).as.letStmt)
#define CREATE_STMT_LET(stmt) ((Stmt){LET_STATEMENT, as.letStmt = &stmt})

typedef enum {
  LET_STATEMENT = 0,
  RETURN_STATEMENT,
} StatementType;

// TODO: Do we need the token type here? we can infer it wil be identifer... and
// do we need to knop the length here?
typedef struct {
  TokenType ttype;
  int length;
  char *value;
} Identifier;

typedef struct {
  Identifier *identifier;
} LetStmt;

typedef struct {
  TokenType type;
} ReturnStatement;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct Stmt {
  StatementType type;
  union {
    LetStmt *letStmt;
    ReturnStatement *returnStmt;
  } as;
  struct Stmt *next;
} Stmt;

typedef struct {
  Stmt *head;
  Stmt *tail;
  size_t length;
} Program;

LetStmt *createLetStmt(Token token);
Program createProgram();
void pushtStmt(Program *program, Stmt *stmt);
Stmt *popStmt(Program *program);
void freeProgram(Program *prog);
void freeIdentifier(Identifier *identifier);

#endif
