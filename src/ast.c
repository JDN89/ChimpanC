#include "ast.h"
#include "dynstring.h"
#include <stdio.h>

LetStmt createLetStmt(Token t) {
  LetStmt letStmt;

  letStmt.identifier = makeString(t.literal, t.length);
  return letStmt;
}

Program createProgram() {
  Program program;
  program.length = 0;
  program.head = NULL;
  program.tail = NULL;
  return program;
}

Stmt *createStmt(StatementType type, void *specificStatement) {
  Stmt *stmt;
  stmt->type = type;
  switch (type) {

  case LET_STATEMENT:

    stmt->as.letStmt = (LetStmt *)specificStatement, stmt->next = NULL;

  default:
    fprintf(stderr, "statment not supported!");
  }

  return stmt;
}

void pushtStmt(Program *program, Stmt *stmt) {
  if (program->head == NULL) {
    program->head = stmt;
    program->tail = stmt;
  } else {
    Stmt *tmp;
    // NOTE: the current tail should point to stmt
    program->tail->next = stmt;
    // NOTE: set tail to stmt
    program->tail = stmt;
    program->tail->next = NULL;
  }
  program->length++;
}
