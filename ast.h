
#include "src/lexer.h"
#include "src/parser.h"
#include <stdio.h>

//NOTE: bothe epxression and STmt have to return the literal value of the token

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

typedef struct String {
  Token token;
  Obj value;
} Id;



struct LetStmt {
  Token token;
  char *identifier; // TODO: wrap in value like in clox (int, string,...)
                    // String object?
};

// TODO: define Expr like this and expr types
// TOOD:place methos for adding pushing and popping the list values
