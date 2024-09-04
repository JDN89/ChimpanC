#include "../src/lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// HELPER METHOD for printing enum values of TokenType
const char *TokenTypeToString(TokenType type) {
  switch (type) {
  case TOKEN_ILLEGAL:
    return "TOKEN_ILLEGAL";
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_INT:
    return "TOKEN_INT";
  case TOKEN_ASSIGN:
    return "TOKEN_ASSIGN";
  case TOKEN_PLUS:
    return "TOKEN_PLUS";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_SEMICOLON:
    return "TOKEN_SEMICOLON";
  case TOKEN_LPAREN:
    return "TOKEN_LPAREN";
  case TOKEN_RPAREN:
    return "TOKEN_RPAREN";
  case TOKEN_LBRACE:
    return "TOKEN_LBRACE";
  case TOKEN_RBRACE:
    return "TOKEN_RBRACE";
  case TOKEN_FUNCTION:
    return "TOKEN_FUNCTION";
  case TOKEN_LET:
    return "TOKEN_LET";
  case TOKEN_GT:
    return ">";
  case TOKEN_LT:
    return "<";
  case TOKEN_ASTERISK:
    return "*";
  case TOKEN_SLASH:
    return "/";
  case TOKEN_MINUS:
    return "-";
  case TOKEN_BANG:
    return "!";
  case TOKEN_FALSE:
    return "false";
  case TOKEN_IF:
    return "if";
  case TOKEN_ELSE:
    return "else";
  case TOKEN_TRUE:
    return "true";
  default:
    return "TOKEN_UNKNOWN";
  }
}

// ==========================
// HELPER FUNCITONS
// Helper function to check token equality with error reporting
void check_token(int position, Token token, TokenType expected_type,
                 const char *expected_literal, int expected_length) {
  if (token.type != expected_type) {
    fprintf(stderr,
            "Error at token position %d: Expected token type %s, but got %s\n",
            position, TokenTypeToString(expected_type),
            TokenTypeToString(token.type));
    assert(token.type == expected_type);
  }

  if (strncmp(token.literal, expected_literal, expected_length) != 0) {
    fprintf(stderr,
            "Error at %d: Expected token literal '%s', but got '%.*s'\n",
            position, expected_literal, token.length, token.literal);
    assert(strncmp(token.literal, expected_literal, expected_length) == 0);
  }

  if (token.length != expected_length) {
    fprintf(stderr, "Error at %d: Expected token length %d, but got %d\n",
            position, expected_length, token.length);
    assert(token.length == expected_length);
  }
}

//============
// TESTS

void bang_test() {
  Lexer lexer;
  char source[] = "!-+ ; !=;\0";

  init_lexer(source, &lexer);

  // TODO: expand test and source code
  TokenType expected_types[] = {TOKEN_BANG,      TOKEN_MINUS,  TOKEN_PLUS,
                                TOKEN_SEMICOLON, TOKEN_NOT_EQ, TOKEN_SEMICOLON,
                                TOKEN_EOF};
  const char *expected_literals[] = {"!", "-", "+", ";", "!=", ";", "\0"};

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
  Lexer lexer;
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

  init_lexer(source, &lexer);

  // TODO: expand test and source code
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
      TOKEN_INT,        TOKEN_NOT_EQ,     TOKEN_INT,        TOKEN_EOF};
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
  printf("Return to monk source test passed.");
}

int main() {
  /*test_monkey_source();*/
  bang_test();

  printf("All tests passed.\n");
  return 0;
}
