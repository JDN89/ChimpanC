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
  Value *value;
} Identifier;

typedef struct {
  char op;
  Expr *right;
} PrefixExpr;

struct Expr {
  ExprType type;
  union {
    Identifier *identifier;
    Value *value;
    PrefixExpr *prefix;
  } as;
};

// WARNING: Not sure if I should wrap value in an Identifier struct?
typedef struct {
  TokenType token; // token.Ident
  Identifier *name;
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
