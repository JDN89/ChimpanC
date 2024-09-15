
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

// NOTE: bothe epxression and STmt have to return the literal value of the token

typedef struct LetStmt LetStmt;
typedef struct Expr Expr;

typedef enum {
  LET_STATEMENT = 0,
} StatementType;

typedef enum { Ident = 0, Operator } ExprType;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct {
  StatementType type;
  union {
    LetStmt *letStmt;
  } as;

} Stmt;

typedef struct {
  Stmt *head;
  Stmt *tail;
  size_t length;
} Program;


struct LetStmt {
  // Token token; Do we need pointer to OG token?
  String *identifier; // TODO: wrap in value like in clox (int, string,...)
};

// TODO: define Expr like this and expr types
// TODO: cast like you did for the objstring to the correct type!!
// TOOD:place methos for adding pushing and popping the list values
