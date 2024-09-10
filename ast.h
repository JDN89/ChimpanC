
#include "src/lexer.h"
#include "src/parser.h"
#include <stdio.h>

typedef struct LetStmt LetStmt;
typedef struct Expr Expr;

typedef enum {
  LET_STATEMENT = 0,
} StatementType;

// NOTE: wrapper for type and pointer to impl of specific statment
typedef struct {
  StatementType type;
  union {
    LetStmt *letStmt;
  };

} Stmt;

typedef struct {
  Stmt *head;
  Stmt *tail;
  size_t length;
} Program;

struct LetStmt {
  Token token;
  char *identifier; // TODO: wrap in value like in clox (int, string,...)
                    // String object?
  Expr value;
};

// TODO: define Expr like this and expr types
