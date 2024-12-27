#ifndef AST_H
#define AST_H

#include "lexer.h"
#include "value.h"
#include <stddef.h>
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
} Statement_Type;

typedef enum {
  IDENTIFIER_EXPR = 0,
  NUMBER_EXPR,
  BOOLEAN_EXPR,
  PREFIX_EXPR,
  INFIX_EXPR,
  IF_EXPR,
  FUNCTION_LITERAL_EXPR,
  CALL_EXPRESSION,
} Expression_Type;

typedef struct {
  Value *value;
} Identifier;

typedef struct {
  char op;
  Expr *right;
} Prefix_Expression;

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

typedef struct {
  size_t count;
  size_t capacity;
  void **elements; // holds generic pointer
} Dyn_Array_Elements;

typedef struct {
  Dyn_Array_Elements *parameters; // stored in dyn array
  Block_Statement *body;
} Function_Literal_Expr;

typedef struct {
  Expr *function_identifier;
  Dyn_Array_Elements *arguments; // stored and dyn array
} Call_Expression;

typedef struct Expr {
  Expression_Type type;
  union {
    Identifier *identifier;
    Value *value;
    Prefix_Expression *prefix;
    Infix_Expression *infix;
    If_Expression *if_expression;
    Function_Literal_Expr *fn;
    Call_Expression *call;
  } as;
} Expr;

// WARNING: Not sure if I should wrap value in an Identifier struct?
// TODO: -> we add idetifier here and remove it from EXPR union
typedef struct {
  TokenType token; // token.Ident
  Identifier *name;
  Expr *expr;
} Let_Statement;

typedef struct {
  TokenType type;
  Expr *expr;
} Return_Statement;

typedef struct {
  Expr *expr;
} Expression_Statement;

typedef struct Block_Statement {
  Token token;
  size_t count;
  size_t capacity;
  Stmt *statements;
} Block_Statement;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct Stmt {
  Statement_Type type;
  union {
    Let_Statement *letStmt;
    Return_Statement *returnStmt;
    Expression_Statement *exprStmt;
    Block_Statement *block_statement;
  } as;
  struct Stmt *next;
} Stmt;

typedef struct {
  Stmt *head;
  Stmt *tail;
  size_t length;
} Program;

Let_Statement *createLetStmt(Token token);
Program createProgram(void);

void push_statement(Program *program, Stmt *stmt);
void freeProgram(Program *prog);
void freeIdentifier(Identifier *identifier);
void create_block_statement(Block_Statement *block_statement);
void write_block_statement(Block_Statement *block, Stmt *statement);
void free_block_statement(Block_Statement *block);
void create_dyn_array(Dyn_Array_Elements *params);
void write_to_function_dyn_array(Dyn_Array_Elements *params, void *element);
void free_dyn_array(Dyn_Array_Elements *params);

#endif
