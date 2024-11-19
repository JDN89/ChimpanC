#include "../src/lexer.h"
#include "test_helper_functions.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// ==========================
// HELPER FUNCITONS

//============
// TESTS

void bang_test() {
  char source[] = "!= ; ==\0";

  Lexer lexer = init_lexer(source);

  TokenType expected_types[] = {TOKEN_NOT_EQ, TOKEN_SEMICOLON, TOKEN_EQ,
                                TOKEN_EOF};
  const char *expected_literals[] = {"!=", ";", "==", "\0"};

  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {
    Token token = nextToken(&lexer);
    check_token(i, token, expected_types[i], expected_literals[i],
                strlen(expected_literals[i]));
    lexer.start = lexer.current;
  }
  printf("Bang my bug");
}

void test_monkey_source() {
  char source[] = "let five = 5;\n"
                  "let ten = 10;\n"
                  "let add = fn(x, y) {\n"
                  "    x + y;\n"
                  "};\n"
                  "let result = add(five, ten); \n"
                  "!-/*5;\n"
                  "5 < 10 > 5; \n"
                  "if(5<10) {\n"
                  "return true \n"
                  "}else { \n"
                  "return false;\n"
                  "} \n"
                  "10 ==10; \n"
                  "10 !=9;  \0";

  Lexer lexer = init_lexer(source);

  TokenType expected_types[] = {
      TOKEN_LET,        TOKEN_IDENTIFIER, TOKEN_ASSIGN,     TOKEN_INT,
      TOKEN_SEMICOLON,  TOKEN_LET,        TOKEN_IDENTIFIER, TOKEN_ASSIGN,
      TOKEN_INT,        TOKEN_SEMICOLON,  TOKEN_LET,        TOKEN_IDENTIFIER,
      TOKEN_ASSIGN,     TOKEN_FUNCTION,   TOKEN_LPAREN,     TOKEN_IDENTIFIER,
      TOKEN_COMMA,      TOKEN_IDENTIFIER, TOKEN_RPAREN,     TOKEN_LBRACE,
      TOKEN_IDENTIFIER, TOKEN_PLUS,       TOKEN_IDENTIFIER, TOKEN_SEMICOLON,
      TOKEN_RBRACE,     TOKEN_SEMICOLON,  TOKEN_LET,        TOKEN_IDENTIFIER,
      TOKEN_ASSIGN,     TOKEN_IDENTIFIER, TOKEN_LPAREN,     TOKEN_IDENTIFIER,
      TOKEN_COMMA,      TOKEN_IDENTIFIER, TOKEN_RPAREN,     TOKEN_SEMICOLON,
      TOKEN_BANG,       TOKEN_MINUS,      TOKEN_SLASH,      TOKEN_ASTERISK,
      TOKEN_INT,        TOKEN_SEMICOLON,  TOKEN_INT,        TOKEN_LT,
      TOKEN_INT,        TOKEN_GT,         TOKEN_INT,        TOKEN_SEMICOLON,
      TOKEN_IF,         TOKEN_LPAREN,     TOKEN_INT,        TOKEN_LT,
      TOKEN_INT,        TOKEN_RPAREN,     TOKEN_LBRACE,     TOKEN_RETURN,
      TOKEN_TRUE,       TOKEN_RBRACE,     TOKEN_ELSE,       TOKEN_LBRACE,
      TOKEN_RETURN,     TOKEN_FALSE,      TOKEN_SEMICOLON,  TOKEN_RBRACE,
      TOKEN_INT,        TOKEN_EQ,         TOKEN_INT,        TOKEN_SEMICOLON,
      TOKEN_INT,        TOKEN_NOT_EQ,     TOKEN_INT,        TOKEN_SEMICOLON,
      TOKEN_EOF};
  const char *expected_literals[] = {
      "let",    "five",   "=",    "5",  ";",    "let", "ten",    "=",     "10",
      ";",      "let",    "add",  "=",  "fn",   "(",   "x",      ",",     "y",
      ")",      "{",      "x",    "+",  "y",    ";",   "}",      ";",     "let",
      "result", "=",      "add",  "(",  "five", ",",   "ten",    ")",     ";",
      "!",      "-",      "/",    "*",  "5",    ";",   "5",      "<",     "10",
      ">",      "5",      ";",    "if", "(",    "5",   "<",      "10",    ")",
      "{",      "return", "true", "}",  "else", "{",   "return", "false", ";",
      "}",      "10",     "==",   "10", ";",    "10",  "!=",     "9",     ";",
      "\0"};

  int num_tokens = sizeof(expected_types) / sizeof(TokenType);

  for (int i = 0; i < num_tokens; i++) {
    Token token = nextToken(&lexer);
    check_token(i, token, expected_types[i], expected_literals[i],
                strlen(expected_literals[i]));
    lexer.start = lexer.current;
  }
  printf("Return to monk source test passed. \n");
}

int main() {
  test_monkey_source();
  printf("\n");
  return 0;
}
