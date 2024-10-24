#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdint.h>
#include <stdio.h>

// NOTE: both epxression and STmt have to return the literal value of the token

#define IS_LET_STMT(stmt) ((stmt)->type == LET_STATEMENT)
#define AS_LET_STMT(stmt) ((stmt).as.letStmt)
#define CREATE_STMT_LET(stmt) ((Stmt){LET_STATEMENT, as.letStmt = &stmt})

typedef enum {
  LET_STATEMENT = 0,
  RETURN_STATEMENT,
  EXPR_STATEMENT,
} StatementType;

/*int64_t*/
typedef struct {
  TokenType ttype;
  int64_t value;
} NumberLiteral;

// TODO: Do we need the token type here? we can infer it wil be identifer... and
// do we need to knop the length here?
typedef struct {
  TokenType ttype;
  int length;
  char *value;
} Identifier;

// TODO: I'll probably have to split this up later
// expr -> union infix, prefix,...
typedef struct {
  union {
    Identifier *identifier;
    NumberLiteral *numberLiteral;
  } as;
} Expr;

typedef struct {
  Expr *expr;
} LetStmt;

typedef struct {
  TokenType type;
} ReturnStatement;


typedef struct {
  Expr *expr;
} ExprStatement;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct Stmt {
  StatementType type;
  union {
    LetStmt *letStmt;
    ReturnStatement *returnStmt;
    ExprStatement *exprStmt;
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
