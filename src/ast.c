#include "ast.h"
#include "value.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void freeExpr(Expr *expr);

// TODO: turn into dynamic array and see if this goes Vrooom
/*Program createProgram() {*/
/*  Program program = {0};*/
/*  return program;*/
/*}*/

// TODO: turn into dynamic array and see if this goes Vrooom
void push_statement(Program *program, Stmt *stmt) {
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

void freePrefix_Expression(Prefix_Expression *pre) { freeExpr(pre->right); }

void free_infix_expression(Infix_Expression *infix) {
  freeExpr(infix->left);
  freeExpr(infix->right);
}
void free_if_expression(If_Expression *exp) {
  freeExpr(exp->condition);
  free_block_statement(exp->alternative);
  free_block_statement(exp->consequence);
}

void freeExpr(Expr *expr) {
  assert(expr != NULL);

  if (expr != NULL) {
    switch (expr->type) {

      // TODO: remove identiferExpr and free_identifier in let statement
    case IDENTIFIER_EXPR: {
      freeIdentifier(expr->as.identifier);
      break;
    }
    case NUMBER_EXPR: {
      freeValue(expr->as.value);
      break;
    }
    case PREFIX_EXPR: {
      freePrefix_Expression(expr->as.prefix);
      break;
    }
    case INFIX_EXPR:
      free_infix_expression(expr->as.infix);
      break;
    case BOOLEAN_EXPR:
      freeValue(expr->as.value);
      break;
    case IF_EXPR:
      free_if_expression(expr->as.if_expression);
      break;
    case FUNCTION_LITERAL_EXPR:
      // @Jan implement
      break;
    case CALL_EXPRESSION:
      // @Jan implement
      break;
    }

    free(expr);
  }
}

void freeLetStmt(Let_Statement *stmt) {
  assert(stmt != NULL);
  if (stmt != NULL) {
    freeExpr(stmt->expr);
    free(stmt);
  }
}

void freeReturnStatement(Return_Statement *stmt) {
  assert(stmt != NULL);
  // TODO: finish function ones we add expressions to return statement
  if (stmt != NULL) {
    freeExpr(stmt->expr);
    free(stmt);
  }
  return;
}

void freeExprStatement(Expression_Statement *stmt) {
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

  case BLOCK_STATEMENT:
    free_block_statement(stmt->as.block_statement);
    break;

  default:
    fprintf(stderr, "Unknown statement type to free.\n");
    break;
  }

  free(stmt); // Free the statement itself
}

void create_block_statement(Block_Statement *block) {
  block->count = 0;
  block->capacity = 0;
  block->statements = NULL;
}
void write_block_statement(Block_Statement *block, Stmt *statement) {
  if (!block || !statement) {
    fprintf(stderr, "Invalid input to write_block_statement.\n");
    return;
  }

  // Resize if capacity is same as count
  if (block->capacity == block->count) {
    size_t new_capacity = block->capacity == 0 ? 8 : block->capacity * 2;
    Stmt *new_statements =
        realloc(block->statements, new_capacity * sizeof(Stmt));
    if (!new_statements) {
      fprintf(stderr, "Memory allocation failed.\n");
      exit(EXIT_FAILURE); // Exit on memory allocation failure
    }

    block->statements = new_statements;
    block->capacity = new_capacity;
  }

  // Add the new statement and increment the count
  block->statements[block->count] = *statement;
  block->count++;
}

void free_block_statement(Block_Statement *block) {
  for (size_t i = 0; i < block->count; i++) {
    freeStmt(&block->statements[i]);
  }
  block->count = 0;
  block->capacity = 0;
  free(block->statements);
  block->statements = NULL;
}

void create_dyn_array(Dyn_Array_Elements *params) {
  // set immediatly the capacity to 8 for efficiency so we don't immediatly have
  // to reallocate
  *params = (Dyn_Array_Elements){0};
}

// use generic void pointers to make the dynamic array more general
void write_to_function_dyn_array(Dyn_Array_Elements *params, void *param) {
  if (!params || !param) {
    fprintf(stderr, "Invalid input for write_to_parameters_list.\n");
    return;
  }
  if (params->capacity == params->count) {
    size_t new_capacity = params->capacity == 0 ? 8 : params->capacity * 2;
    void **new_elements =
        realloc(params->elements, new_capacity * sizeof(void *));

    if (!new_elements) {
      fprintf(stderr,
              "Memory allocation failed in write_to_parameters_list.\n");
      return; // Return early if realloc fails
    }

    params->elements = new_elements;
    params->capacity = new_capacity;
  }
  assert(params->elements != NULL);
  params->elements[params->count] = param;
  params->count++;
}

void free_dyn_array(Dyn_Array_Elements *params) {
  // loop over all the identifiers and free them
  for (size_t i = 0; i < params->count; i++) {
    freeIdentifier(params->elements[i]);
  }
  params->count = 0;
  params->capacity = 0;
  // free the array
  free(params->elements);
  // set the pointer to NULL to avoid dangling pointers
  params->elements = NULL;
  free(params);
}

Program createProgram(void) {
  Program program = {0};
  return program;
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
}
