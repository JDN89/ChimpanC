#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "printer.h"
#include <assert.h>
#include <stdlib.h>

#include <assert.h>
#include <stdio.h>

static void repl();

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

    if (p.errorCount > 0) {
      print_errors(&p);
      free_parser_errors(&p);
    }

    // Print statements
    for (Stmt *currentStmt = program.head; currentStmt != NULL;
         currentStmt = currentStmt->next) {
      assert(currentStmt != NULL); // Ensure statement is valid
      print_statement(currentStmt);
    }

    freeProgram(&program);
  }
}

int main(int argc, char *argv[]) {
  printf("Hello!  This is the Monkey programming language!\n");
  printf("Feel free to type in commands\n");

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    const char *file_name = argv[1];
    printf("Loading file: %s \n", file_name);
    FILE *file = fopen(file_name, "r");
    if (!file) {
      perror("Failed opening file: %s");
      EXIT_FAILURE;
    }
    fclose(file);
    return 1;
  }

  return 1;
}
