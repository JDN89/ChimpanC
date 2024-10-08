#include "../src/lexer.h"
#include "../src/parser.h"
#include "test_helper_functions.h"
#include <stdio.h>

void test_parse_let_statement() {

  char source[] = " let x = 5;\n"
                  " let y = 10;\n"
                  "let foobar = 838383; \n";
  Lexer l = init_lexer(source);
  Parser parser = newParser(&l);
  Program program = parseProgram(&parser);


  /*while (parser.pt.type != TOKEN_EOF) {*/
  /*  fprintf(stdout, "%s  \n", tokenTypeToString(parser.ct.type));*/
  /*  fprintf(stdout, "%s  \n", parser.ct.literal);*/
  /*  advance(&parser);*/
  /*}*/
  /*fprintf(stdout, "%s  \n", tokenTypeToString(parser.ct.type));*/


  printf("Test parse let statement done");
}

int main() { test_parse_let_statement(); }
