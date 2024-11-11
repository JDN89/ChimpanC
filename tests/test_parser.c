#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
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
      printf("Error %d: %s\n", i + 1, parser.errors[i]);
    }
    printf("Report parse error - SUCCESS!\n");
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

  printf("Parse let statement - SUCCESS! \n");
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

  printf("Parse numbers - SUCCESS! \n");
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

  printf("Parse return statement - SUCCESS! \n");
}

//----------------------------------------------------
// TEST parse expressionStatements
//----------------------------------------------------
void test_parse_expressions() {
  char source[] = " let a = 5;\n"
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

  printf("Test expressions - SUCCESS! \n");
}

int main() {
  test_parser_error_during_parse_let_statement();
  test_parse_let_statement();
  test_parse_integer_literal();
  test_parse_return_statement();
  test_parse_expressions();
}
