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
      freeValue(expr->as.value);
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

    freeExpr(stmt->expr);
    assert(stmt != NULL);
    free(stmt);
  }
}

void freeReturnStatement(ReturnStatement *stmt) {
  assert(stmt != NULL);
  // TODO: finish function ones we add expressions to return statement
  if (stmt != NULL) {
    free(stmt);
  }
  return;
}

void freeExprStatement(ExprStatement *stmt) {
  assert(stmt != NULL);
  if (stmt != NULL) {
    freeExpr(stmt->expr);
    free(stmt);
  }
}

void freeStmt(Stmt *stmt) {
  assert(stmt != NULL); // Assert to ensure stmt is not NULL

  switch (stmt->type) {
  case LET_STATEMENT:
    freeLetStmt(stmt->as.letStmt); // Free the let statement structure
    break;

  case RETURN_STATEMENT:
    freeReturnStatement(stmt->as.returnStmt); // Ensure you have a free function
                                              // for return statement
    break;

  case EXPR_STATEMENT:
    freeExprStatement(
        stmt->as.exprStmt); // Free the expression statement structure
    break;

  default:
    fprintf(stderr, "Unknown statement type to free.\n");
    break;
  }

  free(stmt); // Free the statement itself
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
