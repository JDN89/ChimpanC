#ifndef AST_H
#define AST_H

#include "lexer.h"
#include <stdint.h>
#include <stdio.h>

// NOTE: both epxression and STmt have to return the literal value of the token

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

typedef struct {
  // Again, is token necessary. Keep following the book but throw this shit out if you can!
  TokenType token;
  const char *op;
  Expr *right;
} PrefixExpr;

struct Expr {
  ExprType type;
  //TODO: Add Expr type to check that I'm accessing the correct Expresson kind
  union {
    Identifier *identifier;
    NumberLiteral *numberLiteral;
    PrefixExpr *prefix;
  } as;
};

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
