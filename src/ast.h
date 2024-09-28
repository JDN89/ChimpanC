#ifndef AST_H
#define AST_H

#include "dynstring.h"
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

typedef enum { Ident = 0, Operator } ExprType;

typedef struct {
  // Token token; Do we need pointer to OG token?
  String *identifier; // TODO: wrap in value like in clox (int, string,...)
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
#endif
