#include "ast.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

// TODO: turn into dynamic array and see if this goes Vrooom
Program createProgram() {
  Program program;
  program.length = 0;
  program.head = NULL;
  program.tail = NULL;
  return program;
}

// TODO: place creation of letstatemets etc in ast file...
//

void pushtStmt(Program *program, Stmt *stmt) {
  if (program->head == NULL) {
    program->head = stmt;
    program->tail = stmt;
  } else {
    // The current tail should point to stmt
    program->tail->next = stmt;
    // Set tail to stmt
    program->tail = stmt;
    program->tail->next = NULL;
  }
  program->length++;
}

void freeIdentifier(Identifier *identifier) {
  if (identifier != NULL) {
    free(identifier->value);
    free(identifier);
  }
}
void freeLetStmt(LetStmt *stmt) {
  if (stmt != NULL) {
    freeIdentifier(stmt->identifier);
    free(stmt);
  }
}

// TODO: free other statements
void freeStmt(Stmt *stmt) {
  if (stmt != NULL) {
    if (IS_LET_STMT(stmt)) {
      freeLetStmt(stmt->as.letStmt);
    } else {
      free(stmt);
    }
  }
}

void freeProgram(Program *prog) {
  Stmt *current = prog->head;
  while (current != NULL) {
    Stmt *next = current->next;
    freeStmt(current); // Free the current statement
    current = next;    // Move to the next statement
  }
  prog->head = NULL;
  prog->tail = NULL;
  prog->length = 0;
  printf("Program freed!! \n");
}
