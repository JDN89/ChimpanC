#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include <stdio.h>
#include <string.h>

void test_parse_let_statement() {

  char source[] = " let x = 5;\n"
                  " let y = 10;\n"
                  "let foobar = 838383; \n";

  const char *identifiers[] = {"x", "y", "foobar"};

  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  Program program = parseProgram(&parser);

  Stmt *current = program.head;

  int i = 0;
  while (current != NULL) {
    assert(current->type == LET_STATEMENT);
    assert(strcmp(current->as.letStmt->identifier->value, identifiers[i]));
    current = current->next;
    i++;
  }

  printf("Parse let statement SUCCESS! \n");
}

void test_parse_integer_literal() {

  char source[] = "5;  asdfsfd;";
  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  Program program = parseProgram(&parser);

  int64_t expectedInt = 5;

  assert(program.head->as.exprStmt->expr->as.integerLiteral->value ==
         expectedInt);

  printf("Parse IntegerLiteral SUCCESS \n");
}

int main() {
  test_parse_let_statement();

  test_parse_integer_literal();
}
