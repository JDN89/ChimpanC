#include "ast.h"
#include "dynstring.h"
#include <stdio.h>
#include <stdlib.h>

LetStmt createLetStmt(Token t) {
  LetStmt letStmt;

  letStmt.identifier = makeString(t.literal, t.length);
  return letStmt;
}

// can't we turn this into an array?
Program createProgram() {
  Program program;
  program.length = 0;
  program.head = NULL;
  program.tail = NULL;
  return program;
}



void pushtStmt(Program *program, Stmt *stmt) {
  if (program->head == NULL) {
    program->head = stmt;
    program->tail = stmt;
  } else {
    // NOTE: the current tail should point to stmt
    program->tail->next = stmt;
    // NOTE: set tail to stmt
    program->tail = stmt;
    program->tail->next = NULL;
  }
  program->length++;
}
