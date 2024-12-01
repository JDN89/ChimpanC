#include "write_output_to_buffer.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_buffer(Buffer *buffer) {
  assert(buffer != NULL);
  buffer->size = 0;
  buffer->capacity = sizeof(buffer->data);
}

void reset_buffer(Buffer *buffer) {
  buffer->size = 0;
  memset(buffer->data, 0, sizeof(buffer->data));
}

void append_char_to_buffer(Buffer *buffer, char ch) {
  assert(buffer != NULL);

  if (buffer->size + 1 > buffer->capacity) {
    fprintf(stderr, "Error: Buffer overflow detected\n");
    exit(EXIT_FAILURE); // Exit or handle the error gracefully.
  }

  buffer->data[buffer->size] = ch;
  buffer->size++;
  buffer->data[buffer->size] = '\0'; // Null-terminate.
}

void append_string_to_buffer(Buffer *buffer, char *source) {
  assert(buffer != NULL);

  size_t len = strlen(source);
  if (buffer->size + len + 1 > buffer->capacity) {
    fprintf(stderr, "Error: Buffer overflow detected\n");
    exit(EXIT_FAILURE); // Exit or handle the error gracefully.
  }

  memcpy(buffer->data + buffer->size, source, len); // Append new data.
  buffer->size += len;
  buffer->data[buffer->size] = '\0'; // Null-terminate.
}
void print_buffer(Buffer *buffer) { printf("%s", buffer->data); }

void print_expression(Buffer *buffer, Expr *ex);

void print_number(Buffer *buffer, double num) {
  assert(buffer != NULL);

  char temp[64]; // Large enough for most double representations
  snprintf(temp, sizeof(temp), "%g", num); // convert number to string
  append_string_to_buffer(buffer, temp);
}

void print_boolean(Buffer *buffer, bool boolean) {
  if (boolean == true) {
    append_string_to_buffer(buffer, "true");
  } else {
    append_string_to_buffer(buffer, "false");
  }
}

void print_string(ObjString *str) { printf("String - %s \n", str->pointer); }

void print_identifier_name(Buffer *buffer, Identifier *identifier) {
  assert(identifier != NULL);
  switch (identifier->value->type) {
  case VAL_NUMBER: {
    assert(identifier->value->as.number != 0);
    print_number(buffer, identifier->value->as.number);
    break;
  }
  case VAL_STRING:
    assert(identifier->value->as.string != NULL);
    print_string(identifier->value->as.string);
    break;
  case VAL_BOOL:
    print_boolean(buffer, identifier->value->as.boolean);
    break;
  }
}

void print_value(Buffer *buffer, Value *val) {
  assert(val != NULL);
  switch (val->type) {
  case VAL_NUMBER: {
    assert(val->as.number != 0);
    print_number(buffer, val->as.number);
    break;
  }

  case VAL_STRING:
    assert(val->as.string != NULL);
    print_string(val->as.string);
  case VAL_BOOL:
    print_boolean(buffer, val->as.boolean);
    break;
    break;
  }
}

void print_prefix_expression(Buffer *buffer, PrefixExpr *pre) {
  append_string_to_buffer(buffer, "(");
  // BUG: found the bug -> we where passing this as an adress
  append_char_to_buffer(buffer, pre->op);
  print_expression(buffer, pre->right);
  append_string_to_buffer(buffer, ")");
}

void print_infix_expression(Buffer *buffer, Infix_Expression *ex) {
  append_string_to_buffer(buffer, "(");
  print_expression(buffer, ex->left);

  append_string_to_buffer(buffer, ex->op);
  print_expression(buffer, ex->right);
  append_string_to_buffer(buffer, ")");
}

void print_expression(Buffer *buffer, Expr *ex) {
  assert(ex != NULL);

  switch (ex->type) {

  case IDENTIFIER_EXPR:
    print_identifier_name(buffer, ex->as.identifier);
    break;

  case NUMBER_EXPR: {
    assert(ex->as.value->type == VAL_NUMBER);
    print_value(buffer, ex->as.value);
    break;
  }
  case PREFIX_EXPR:
    assert(ex->as.prefix != NULL);
    print_prefix_expression(buffer, ex->as.prefix);
    break;
  case INFIX_EXPR: {
    print_infix_expression(buffer, ex->as.infix);
  } break;
  case BOOLEAN_EXPR:
    assert(ex->as.value->type == VAL_BOOL);
    print_value(buffer, ex->as.value);
    break;
  }
}

void print_let_statement(Buffer *buffer, LetStmt *st) {
  assert(st != NULL);
  assert(st->name != NULL);
  print_identifier_name(buffer, st->name);
  assert(st->expr != NULL);
  print_expression(buffer, st->expr);
}

void print_expression_statement(Buffer *buffer, ExprStatement *st) {
  assert(st != NULL);
  assert(st->expr != NULL);
  print_expression(buffer, st->expr);
}

Buffer *write_statement(Stmt *stmt);

void write_statement_to_output(Buffer *buffer, Stmt *stmt) {
  assert(stmt != NULL);
  switch (stmt->type) {

  case LET_STATEMENT: {
    print_let_statement(buffer, stmt->as.letStmt);
    break;
  }
  case RETURN_STATEMENT: {
    printf("return statement \n");
    break;
  }

  case EXPR_STATEMENT:
    print_expression_statement(buffer, stmt->as.exprStmt);
    break;
  }
}
