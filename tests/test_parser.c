#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include "write_output_to_buffer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

bool test_value(Value parsed, Value expected) {
  switch (parsed.type) {
  case VAL_NUMBER:
    printf("number -%f \n", parsed.as.number);
    return parsed.as.number == AS_NUMBER(expected);
  case VAL_STRING:
    return strcmp(AS_CSTRING(parsed), AS_CSTRING(expected)) == 0;
  case VAL_BOOL:
    return AS_BOOLEAN(parsed) == AS_BOOLEAN(expected);
  }
  return false;
}

bool check_errors(Parser *parser, const char *test_name) {
  if (parser->errorCount > 0) {
    printf("Errors in test: %s - ", test_name);
    for (int i = 0; i < parser->errorCount; i++) {
      printf("Error %d: %s", i + 1, parser->errors[i]);
    }
    return true;
  }
  return false;
}

void print_erros(Parser *parser, const char *test_name) {
  if (parser->errorCount > 0) {
    printf("Errors in test: %s - ", test_name);

    for (int i = 0; i < parser->errorCount; i++) {
      printf("Error %d: %s", i + 1, parser->errors[i]);
    }
    printf("Report parse error - PASSED!\n");
  } else {
    printf("Report parser error - FAILED!\n");
  }
}

void test_parser_error_during_parse_let_statement() {
  // GIVEN:
  char source[] = "let x & 5;\n";

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  // WHEN:
  parse_program(&parser);

  // THEN:
  print_erros(&parser, "test_parser_errors");
}

//----------------------------------------------------
// TEST parse let statement
//----------------------------------------------------

void test_parse_let_statement() {

  char source[] = " let x = 5;\n"
                  " let y = 10;\n"
                  "let foobar = 838383; \n"
                  "let yolo = true \n";

  const char *identifiers[] = {"x", "y", "foobar", "yolo"};
  const Value values[] = {NUMBER(5), NUMBER(10), NUMBER(838383), BOOLEAN(true)};

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int i = 0;

  if (check_errors(&parser, "Test parse Let statements")) {
    printf("Parse let statement - FAILED! \n");
    return;
  }

  while (current != NULL) {
    assert(current->type == LET_STATEMENT);

    assert(test_value(*current->as.letStmt->expr->as.value, values[i]) == true);

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

  if (check_errors(&parser, "Test parse integers")) {
    printf("Parse numbers - FAILED! \n");
    return;
  }

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

  if (check_errors(&parser, "Test parse return statements")) {
    printf("Parse return statement - FAILED! \n");
    return;
  }

  while (current != NULL) {
    assert(current->type == RETURN_STATEMENT);
    assert(current->as.returnStmt->type == TOKEN_RETURN);
    current = current->next;
  }

  printf("Parse return statement - PASSED! \n");
}

//----------------------------------------------------
// TEST parse values
//----------------------------------------------------
void test_parse_values() {
  char source[] = "34 ; \n"
                  "true ;\n";

  Value values[] = {NUMBER(34), BOOLEAN(true)};

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int values_index = 0;

  if (check_errors(&parser, "Test parse expression statements")) {
    printf("Parse expression statement - FAILED! \n");
    return;
  }
  while (current != NULL) {

    assert(current->type == EXPR_STATEMENT);
    test_value(*current->as.exprStmt->expr->as.value, values[values_index]);
    values_index++;
    current = current->next;
  }
  printf("Test parsing values - PASSED! \n");
}

//----------------------------------------------------
// TEST parse expressionStatements
//----------------------------------------------------
void test_parse_expressions() {
  char source[] = " let a = 5;\n"
                  " yolo; \n"
                  " return 10;\n"
                  " 34; \n"
                  " true; \n"
                  " false; \n";

  const char *let_statement_identifiers[] = {"a"};
  const char *identifiers[] = {"yolo"};
  Value values[] = {NUMBER(10), NUMBER(34), BOOLEAN(true), BOOLEAN(false)};

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int letIndex = 0;
  int identifierIndex = 0;
  int values_index = 0;

  if (check_errors(&parser, "Test parse expression statements")) {
    printf("Parse expression statement - FAILED! \n");
    return;
  }

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
      values_index++;
    } else if (current->type == EXPR_STATEMENT) {
      // Test expression statement
      assert(current->type == EXPR_STATEMENT);
      assert(test_value(*current->as.exprStmt->expr->as.value,
                        values[values_index]) == true);
      values_index++;
    }
    current = current->next;
  }

  printf("Test parse expression statements - PASSED! \n");
}

void parse_prefix_expressions() {

#define PREFIX_TEST_CASE_COUNT 4

  typedef struct {
    char *input;
    char expected_op;
    Value expected_value;
  } Prefix_Test;

  Prefix_Test test[PREFIX_TEST_CASE_COUNT] = {{"!true", '!', BOOLEAN(true)},
                                              {"!5", '!', NUMBER(5)},
                                              {"!false", '!', BOOLEAN(false)},
                                              {"-5", '-', NUMBER(5)}};

  for (uint8_t i = 0; i < PREFIX_TEST_CASE_COUNT; i++) {

    Lexer l = init_lexer(test[i].input);
    Parser parser = new_parser(&l);
    Program program = parse_program(&parser);

    if (parser.errorCount > 0) {
      for (int j = 0; j < parser.errorCount; j++) {
        printf("Error %d: %s\n", j + 1, parser.errors[j]);
      }
    }

    Stmt *current = program.head;
    while (current != NULL) {
      assert(current->as.exprStmt->expr->as.prefix->op ==
             current->as.exprStmt->expr->as.prefix->op);
      assert(test_value(*current->as.exprStmt->expr->as.prefix->right->as.value,
                        test[i].expected_value));

      current = current->next;
    }
    freeProgram(&program);
  }
  printf("parse prefix statements - PASSED!\n");
}

// TODO: remove duplicate code
void test_parse_infix_expressions() {
#define INFIX_TEST_CASE_COUNT 20
  typedef struct {
    char *input;
    char *output;
  } Infix_Test;

  Infix_Test test[INFIX_TEST_CASE_COUNT] = {
      {"5+5-5", "((5+5)-5)"},
      {"5+5*5", "(5+(5*5))"},
      {"5+5-5", "((5+5)-5)"},
      {"5+5", "(5+5)"},
      {"5*5", "(5*5)"},
      {"5+5", "(5+5)"},
      {"5-5;", "(5-5)"},
      {"5==5;", "(5==5)"},
      {"5!=5;", "(5!=5)"},
      {"5-5;", "(5-5)"},
      {"5/5;", "(5/5)"},
      {"5>5;", "(5>5)"},
      {"5<5;", "(5<5)"},
      {"3>5 == false", "((3>5)==false)"},
      {"3<5 == true", "((3<5)==true)"},
      {"1+(2 + 3) + 4", "((1+(2+3))+4)"},
      {"(5+5)*2", "((5+5)*2)"},
      {"2/(5+5)", "(2/(5+5))"},
      {"-(5+5)", "(-(5+5))"},
      {"!(true == true)", "(!(true==true))"}};

  for (uint8_t i = 0; i < INFIX_TEST_CASE_COUNT; i++) {
    // INFO: DEBUG
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
      // INFO: DEBUG
      /*printf("Parsed expression  : %s\n", buffer.data);*/

      assert(strcmp(test[i].output, buffer.data) == 0); // Debug if this
      reset_buffer(&buffer);
      current = current->next;
    }
    freeProgram(&program);
  }
  printf("parse infix statements - PASSED!\n");
}

void test_parse_if_statement() {

  char source[] = " if(x==5)\n"
                  "{let t = 10; \n}"
                  "else {y}";

  const char *identifiers[] = {"x", "t", "y"};
  const Value values[] = {
      NUMBER(5),
      NUMBER(10),
  };

  Lexer l = init_lexer(source);
  Parser parser = new_parser(&l);
  Program program = parse_program(&parser);

  Stmt *current = program.head;
  int i = 0;

  if (check_errors(&parser, "Test parse if statements")) {
    print_erros(&parser, "parse if statement");
    printf("Parse if statement - FAILED! \n");
    return;
  }

  assert(current->type == EXPR_STATEMENT);
  assert(current->as.exprStmt->expr->type == IF_EXPR);
  If_Expression *if_e = current->as.exprStmt->expr->as.if_expression;
  assert(if_e->condition->as.infix->right->as.value->as.number ==
         values[0].as.number);

  assert(strcmp(if_e->condition->as.infix->left->as.identifier->value->as
                    .string->pointer,
                identifiers[0]) == 0);
  printf(" consequence -- %s \n",
         if_e->consequence->statements[if_e->consequence->count]
             .as.letStmt->name->value->as.string->pointer);
  assert(strcmp(if_e->consequence->statements[if_e->consequence->count]
                    .as.letStmt->name->value->as.string->pointer,
                identifiers[1]) == 0);

  printf("Parse if statement - PASSED! \n");
}

int main() {
  test_parser_error_during_parse_let_statement();
  test_parse_let_statement();
  test_parse_integer_literal();
  test_parse_return_statement();
  test_parse_expressions();
  test_parse_infix_expressions();
  test_parse_values();
  parse_prefix_expressions();
  test_parse_if_statement();
  printf("\n");
  return 0;
}
