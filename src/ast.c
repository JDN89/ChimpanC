#include "ast.h"
#include "value.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void freeExpr(Expr *expr);

// TODO: turn into dynamic array and see if this goes Vrooom
Program createProgram() {
  Program program = {0};
  return program;
}

void pushtStmt(Program *program, Stmt *stmt) {
  assert(program != NULL && stmt != NULL); // Safety check for NULL pointers
  
    // Initialize the `next` pointer of the new `stmt` to NULL
    stmt->next = NULL;

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
  assert(identifier != NULL);

  if (identifier != NULL) {
    freeValue(identifier->value);
    free(identifier);
  }
}

void freePrefixExpr(PrefixExpr *pre) { freeExpr(pre->right); }

void freeExpr(Expr *expr) {
  printf("in free expression!");
  assert(expr != NULL);

  if (expr != NULL) {
    switch (expr->type) {

    case IDENTIFIER_EXPR: {
      freeIdentifier(expr->as.identifier);
      break;
    }
    case NUMBER_EXPR: {
      printf("Number expr?");
      freeValue(expr->as.value);
      free(expr);
      break;
    }
    case PREFIX_EXPR: {
      freePrefixExpr(expr->as.prefix);
      break;
    }
    }

    free(expr);
  }
}

void freeLetStmt(LetStmt *stmt) {
  if (stmt != NULL) {

    freeExpr(stmt->value);
    assert(stmt != NULL);
    free(stmt);
  }
}

// TODO: free other statements
void freeStmt(Stmt *stmt) {
  assert(stmt != NULL);
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
    if (current != NULL) {
      current = next; // Move to the next statement
    } else {
      break;
    }
  }
  prog->head = NULL;
  prog->tail = NULL;
  prog->length = 0;
  printf("Program freed!! \n");
}
