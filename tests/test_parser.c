#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include <stdio.h>

void test_parse_let_statement() {

  char source[] = " let x = 5;\n"
                  " let y = 10;\n"
                  "let foobar = 838383 \n";
  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  while (parser.peekToken.type != TOKEN_EOF) {
    fprintf(stdout, "%s  \n", TokenTypeToString(parser.curToken.type));
    getToken(&parser);
  }
  fprintf(stdout, "%s  \n", TokenTypeToString(parser.curToken.type));
}

int main() { test_parse_let_statement(); }
