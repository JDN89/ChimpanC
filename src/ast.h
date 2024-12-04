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
typedef struct Stmt Stmt;
typedef struct Block_Statement Block_Statement;

typedef enum {
  LET_STATEMENT = 0,
  RETURN_STATEMENT,
  EXPR_STATEMENT,
  BLOCK_STATEMENT,
} StatementType;

typedef enum {
  IDENTIFIER_EXPR = 0,
  NUMBER_EXPR,
  BOOLEAN_EXPR,
  PREFIX_EXPR,
  INFIX_EXPR,
  IF_EXPR
} ExprType;

typedef struct {
  Value *value;
} Identifier;

typedef struct {
  char op;
  Expr *right;
} PrefixExpr;

typedef struct {
  Expr *left;
  char op[3];
  Expr *right;
} Infix_Expression;

typedef struct {
  Token token;
  Expr *condition;
  Block_Statement *consequence;
  Block_Statement *alternative;
} If_Expression;

typedef struct Expr {
  ExprType type;
  union {
    Identifier *identifier;
    Value *value;
    PrefixExpr *prefix;
    Infix_Expression *infix;
    If_Expression *if_expression;
  } as;
} Expr;

// WARNING: Not sure if I should wrap value in an Identifier struct?
// TODO: -> we add idetifier here and remove it from EXPR union
typedef struct {
  TokenType token; // token.Ident
  Identifier *name;
  Expr *expr;
} LetStmt;

typedef struct {
  TokenType type;
  Expr *expr;
} ReturnStatement;

typedef struct {
  Expr *expr;
} ExprStatement;

typedef struct Block_Statement {
  Token token;
  size_t count;
  size_t capacity;
  Stmt *statements;
} Block_Statement;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct Stmt {
  StatementType type;
  union {
    LetStmt *letStmt;
    ReturnStatement *returnStmt;
    ExprStatement *exprStmt;
    Block_Statement *block_statement;
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
void create_block_statement(Block_Statement *block_statement);
void write_block_statement(Block_Statement *block, Stmt *statement);
void free_block_statement(Block_Statement *block, Stmt *statement);

#endif
