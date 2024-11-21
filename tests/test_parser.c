#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include "write_output_to_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void test_parser_error_during_parse_let_statement() {

  // GIVEN:
  char source[] = "let x & 5;\n";

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  // WHEN:
  parse_program(&parser);

  // THEN:
  if (parser.errorCount > 0) {

    for (int i = 0; i < parser.errorCount; i++) {
      printf("Error %d: %s", i + 1, parser.errors[i]);
    }
    printf("Report parse error - PASSED!\n");
  } else {
    printf("Report parser error - FAILED!\n");
  }
}

//----------------------------------------------------
// TEST parse let statement
//----------------------------------------------------
void test_parse_let_statement() {

  char source[] = " let x = 5;\n"
                  " let y = 10;\n"
                  "let foobar = 838383; \n";

  const char *identifiers[] = {"x", "y", "foobar"};
  const double values[] = {5, 10, 838383};

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int i = 0;

  while (current != NULL) {
    assert(current->type == LET_STATEMENT);
    assert(strcmp(current->as.letStmt->name->value->as.string->pointer,
                  identifiers[i]) == 0);
    assert(current->as.letStmt->expr->as.value->as.number == values[i]);

    current = current->next;
    i++;
  }

  printf("Parse let statement - PASSED! \n");
}

//----------------------------------------------------
// TEST parse numbers
//----------------------------------------------------
void test_parse_integer_literal() {

  char source[] = " 4; \n"
                  " 55; \n"
                  "345345354345";

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

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

  printf("Parse numbers - PASSED! \n");
}

//----------------------------------------------------
// TEST parse return statements
//----------------------------------------------------
void test_parse_return_statement() {

  char source[] = " return yolo  ; \n"
                  " return    55; \n"
                  "return  345345354345; \n";

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;

  while (current != NULL) {
    assert(current->type == RETURN_STATEMENT);
    assert(current->as.returnStmt->type == TOKEN_RETURN);
    current = current->next;
  }

  printf("Parse return statement - PASSED! \n");
}

//----------------------------------------------------
// TEST parse expressionStatements
//----------------------------------------------------
void test_parse_expressions() {
  char source[] = 
    " let a = 5;\n"
                  " yolo; \n"
                  " return 10;\n"
                  " 34; \n";

  const char *let_statement_identifiers[] = {"a"};
  const char *identifiers[] = {"yolo"};
  int64_t numbers[] = {10, 34};

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int letIndex = 0;
  int identifierIndex = 0;
  int numberIndex = 0;

  while (current != NULL) {
    if (current->type == LET_STATEMENT) {
      // Test let statement
      assert(current->type == LET_STATEMENT);
      assert(strcmp(current->as.letStmt->name->value->as.string->pointer,
                    let_statement_identifiers[letIndex]) == 0);
      letIndex++;

    } else if (current->type == EXPR_STATEMENT &&
               current->as.exprStmt->expr->type == IDENTIFIER_EXPR) {

      assert(strcmp(current->as.exprStmt->expr->as.identifier->value->as.string
                        ->pointer,
                    identifiers[identifierIndex]) == 0);
      identifierIndex++;
    }

    else if (current->type == RETURN_STATEMENT) {
      // Test return statement
      assert(current->type == RETURN_STATEMENT);
      assert(current->as.returnStmt->type == TOKEN_RETURN);
      numberIndex++;
    } else if (current->type == EXPR_STATEMENT) {
      // Test expression statement
      assert(current->type == EXPR_STATEMENT);
      assert(current->as.exprStmt->expr->type == NUMBER_EXPR);
      assert(current->as.exprStmt->expr->as.value->as.number ==
             numbers[numberIndex]);
      numberIndex++;
    }
    current = current->next;
  }

  printf("Test expressions - PASSED! \n");
}

void test_parse_infix_expressions() {
#define INFIX_TEST_CASE_COUNT 13
  typedef struct {
    char *input;
    char *output;
  } Infix_Test;

  Infix_Test test[INFIX_TEST_CASE_COUNT] = {
      {"5+5-5", "((5+5)-5)"}, {"5+5*5", "(5+(5*5))"}, {"5+5-5", "((5+5)-5)"},
      {"5+5", "(5+5)"},       {"5*5", "(5*5)"},       {"5+5", "(5+5)"},
      {"5-5;", "(5-5)"},      {"5==5;", "(5==5)"},    {"5!=5;", "(5!=5)"},
      {"5-5;", "(5-5)"},      {"5/5;", "(5/5)"},      {"5>5;", "(5>5)"},
      {"5<5;", "(5<5)"},
  };

  for (uint8_t i = 0; i < INFIX_TEST_CASE_COUNT; i++) {
    // DEBUG INFO
    /*printf("Test Case #%d\n", i + 1);*/
    /*printf("Input   : %s\n", test[i].input);*/
    /*printf("Expected: %s\n", test[i].output);*/

    Lexer l = init_lexer(test[i].input);
    Parser parser = new_parser(&l);
    Program program = parse_program(&parser);

    if (parser.errorCount > 0) {
      for (int j = 0; j < parser.errorCount; j++) {
        printf("Error %d: %s\n", j + 1, parser.errors[j]);
      }
    }

    Buffer buffer;
    init_buffer(&buffer);

    Stmt *current = program.head;
    while (current != NULL) {
      write_statement_to_output(&buffer, current);
      // DEBUG INFO
      /*printf("Parsed expression  : %s\n", buffer.data);*/

      assert(strcmp(test[i].output, buffer.data) == 0); // Debug if this
      reset_buffer(&buffer);
      current = current->next;
    }
    freeProgram(&program);
  }
  printf("parse infix statements - PASSED!\n");
}

int main() {
  test_parser_error_during_parse_let_statement();
  /*test_parse_let_statement();*/
  test_parse_integer_literal();
  test_parse_return_statement();
  test_parse_expressions();
  test_parse_infix_expressions();
  printf("\n");
  return 0;
}
