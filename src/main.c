#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "printer.h"
#include <assert.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

static void processStatement(Stmt *currentStmt);
static void repl();
static void processLetStatement(Stmt *currentStmt);
static void processExprStatement(Stmt *currentStmt);

// TODO: create repl h and c file
static void repl() {
  char line[1024];

  // Infinite loop for reading and processing input
  for (;;) {
    printf(">> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }

    Lexer l = init_lexer(line);
    Parser p = new_parser(&l);
    Program program = parse_program(&p);

    // Process each statement in the program
    for (Stmt *currentStmt = program.head; currentStmt != NULL;
         currentStmt = currentStmt->next) {
      assert(currentStmt != NULL); // Ensure statement is valid
      print_statement(currentStmt);
      /*processStatement(currentStmt);*/
    }

    // Free resources after processing
    if (p.errorCount > 0) {
      freeParserErrors(&p);
    }

    freeProgram(&program);
  }
}

int main(int argc, const char *argv[]) {
  printf("Hello!  This is the Monkey programming language!\n");
  printf("Feel free to type in commands\n");

  if (argc == 1) {
    repl();
  }

  return 0;
}
