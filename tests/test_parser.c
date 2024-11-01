#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include <stdio.h>
#include <string.h>

void test_parser_error_during_parse_let_statement() {

  // GIVEN:
  char source[] = "let x & 5;\n";

  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  // WHEN:
  Program program = parseProgram(&parser);

  // THEN:
  if (parser.errorCount > 0) {

    for (int i = 0; i < parser.errorCount; i++) {
      printf("Error %d: %s\n", i + 1, parser.errors[i]);
    }
    printf("Report parse error - SUCCESS!\n");
  } else {
    printf("Report parser error - FAILED!\n");
  }
}

//-----------------
// TEST parse let statement
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
    /*printf("Parsed identifier: %s\n",*/
    /*       current->as.letStmt->value->as.identifier->value->as.string->pointer);*/
    /*printf("Expected identifier: %s\n", identifiers[i]);*/
    assert(current->type == LET_STATEMENT);
    assert(current->as.letStmt->value->type == IDENTIFIER_EXPR);
    assert(strcmp(current->as.letStmt->value->as.identifier->value->as.string->pointer,
                  identifiers[i]) == 0);
    current = current->next;
    i++;
  }

  printf("Parse let statement - SUCCESS! \n");
}

void test_parse_integer_literal() {

  char source[] = " 4; \n"
                  " 55; \n"
                  "345345354345";

  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  Program program = parseProgram(&parser);

  int64_t expected[] = {4, 55, 345345354345};

  Stmt *current = program.head;
  int i = 0;

  while (current != NULL) {
    assert(current->type == EXPR_STATEMENT);
    assert(current->as.exprStmt->expr->type == NUMBER_EXPR);
    assert(current->as.exprStmt->expr->as.value->as.number == expected[i]);
    current = current->next;
    i++;
  }

  printf("Parse numbers - SUCCESS! \n");
}

int main() {
  test_parser_error_during_parse_let_statement();
  test_parse_let_statement();
  test_parse_integer_literal();
}
