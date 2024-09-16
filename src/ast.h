
#include "dynstring.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>

// NOTE: bothe epxression and STmt have to return the literal value of the token

typedef struct LetStmt LetStmt;
typedef struct Expr Expr;

#define IS_LET_STMT(stmt) ((stmt)type == LET_STATEMENT);
#define AS_LET_STMT(stmt) ((stmt).as.letStmt);
#define CREATE_STMT_LET(stmt) ((Stmt){LET_STATEMENT, as.letStmt = &stmt})

typedef enum {
  LET_STATEMENT = 0,
} StatementType;

typedef enum { Ident = 0, Operator } ExprType;

// NOTE: wrapper for type and pointer to impl of specific statment

typedef struct Stmt {
  StatementType type;
  union {
    LetStmt *letStmt;
  } as;
  struct Stmt *next;
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

LetStmt createLetStmt(Token token);
Stmt *createStmt(StatementType type, void *specificStatement);
Program createProgram();
void pushtStmt(Program *program, Stmt *stmt);
Stmt *popStmt(Program *program);
