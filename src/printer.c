#include "printer.h"
#include "ast.h"
#include "value.h"

#include <assert.h>
#include <stdio.h>

void print_expression(Expr *ex);

void print_number(double num) { printf("%f", num); }
void print_string(ObjString *str) { printf("String - %s \n", str->pointer); }

void print_identifier_name(Identifier *identifier) {
  assert(identifier != NULL);
  switch (identifier->value->type) {
  case VAL_NUMBER: {
    assert(identifier->value->as.number != 0);
    print_number(identifier->value->as.number);
    break;
  }
  case VAL_STRING:
    assert(identifier->value->as.string != NULL);
    print_string(identifier->value->as.string);
    break;
  case VAL_BOOL:
    fprintf(stderr, "Boolean can't be an identifier!");
    break;
  }
}

void print_value(Value *val) {
  assert(val != NULL);
  switch (val->type) {
  case VAL_NUMBER: {
    assert(val->as.number != 0);
    print_number(val->as.number);
    break;
  }

  case VAL_STRING:
    assert(val->as.string != NULL);
    print_string(val->as.string);
    break;
  case VAL_BOOL:
    if (val->as.boolean == true) {
      printf("true");
    } else {
      printf("false");
    }
    break;
  }
}

void print_prefix_expression(Prefix_Expression *pre) {
  printf("prefix -- %c", pre->op);
  assert(pre->right != NULL);
  print_expression(pre->right);
}

void print_infix_expression(Infix_Expression *ex) {
  printf("(");
  print_expression(ex->left);
  printf(" %s ", ex->op);
  print_expression(ex->right);
  printf(")");
}

void print_if_expression(If_Expression *if_expression) {
  printf("if (");
  switch (if_expression->condition->type) {
  case IDENTIFIER_EXPR:
    // @Jan print error because normally we won't hit this path
    print_identifier_name(if_expression->condition->as.identifier);
    break;
  case NUMBER_EXPR:
    print_number(if_expression->condition->as.value->as.number);
    break;
  case PREFIX_EXPR:
    print_prefix_expression(if_expression->condition->as.prefix);
    break;
  case INFIX_EXPR:
    print_infix_expression(if_expression->condition->as.infix);
    break;
  case IF_EXPR:
    fprintf(stderr,
            "We shouldn't hit this branch inside the if_expression printer!\n");
    break;
  case BOOLEAN_EXPR:
    print_value(if_expression->condition->as.value);
    break;
  case FUNCTION_LITERAL_EXPR:
    //@Jan implement
    break;
  }
  printf(") {");
}

void print_expression(Expr *ex) {
  assert(ex != NULL);

  switch (ex->type) {

  case IDENTIFIER_EXPR:
    print_identifier_name(ex->as.identifier);
    break;

  case NUMBER_EXPR: {
    assert(ex->as.value->type == VAL_NUMBER);
    print_value(ex->as.value);
    break;
  }
  case PREFIX_EXPR:
    assert(ex->as.prefix != NULL);
    print_prefix_expression(ex->as.prefix);
    break;
  case INFIX_EXPR: {
    print_infix_expression(ex->as.infix);
  } break;
  case IF_EXPR:
    print_if_expression(ex->as.if_expression);
    break;
  case BOOLEAN_EXPR:
    print_value(ex->as.value);
    break;
  case FUNCTION_LITERAL_EXPR:
    //@Jan implement
    break;
  }
}

void print_let_statement(Let_Statement *st) {
  assert(st != NULL);
  assert(st->name != NULL);
  print_identifier_name(st->name);
  assert(st->expr != NULL);
  print_expression(st->expr);
}

void print_expression_statement(Expression_Statement *st) {
  assert(st != NULL);
  assert(st->expr != NULL);
  print_expression(st->expr);
}

void print_statement(Stmt *stmt) {
  assert(stmt != NULL);
  switch (stmt->type) {

  case LET_STATEMENT: {
    print_let_statement(stmt->as.letStmt);
    break;
  }
  case RETURN_STATEMENT: {
    printf("return statement \n");
    break;
  }

  case EXPR_STATEMENT:
    print_expression_statement(stmt->as.exprStmt);
    break;
    //@Jan Todo implement
  case BLOCK_STATEMENT:
    break;
  }
}

void print_errors(Parser *parser) {
  if (parser->errorCount > 0) {

    for (int i = 0; i < parser->errorCount; i++) {
      printf("Error %d: %s\n", i + 1, parser->errors[i]);
    }
    printf("Report parse error - PASSED!\n");
  } else {
    printf("Report parser error - FAILED!\n");
  }
}
