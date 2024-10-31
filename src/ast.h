#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "value.h"
#include <stdint.h>
#include <stdio.h>

#define IS_LET_STMT(stmt) ((stmt)->type == LET_STATEMENT)
#define AS_LET_STMT(stmt) ((stmt).as.letStmt)
#define CREATE_STMT_LET(stmt) ((Stmt){LET_STATEMENT, as.letStmt = &stmt})

typedef struct Expr Expr;

typedef enum {
  LET_STATEMENT = 0,
  RETURN_STATEMENT,
  EXPR_STATEMENT,
} StatementType;

typedef enum {
  IDENTIFIER_EXPR = 0,
  NUMBER_EXPR,
  PREFIX_EXPR,
} ExprType;

typedef struct {
  const char *op;
  Expr *right;
} PrefixExpr;

struct Expr {
  ExprType type;
  union {
    Value *value;
    PrefixExpr *prefix;
  } as;
};

// WARNING: Not sure if I should wrap value in an Identifier struct?
typedef struct {
  TokenType token; // token.Ident
  Value *name;
  Expr *value;
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
void freeIdentifier(Value *identifier);

#endif
