#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

static void repl() {
  char line[1024];

  // Infinit loop for reading and precessing input
  for (;;) {
    printf(">> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    Lexer l = init_lexer(line);
    Parser p = newParser(&l);
    Program program = parseProgram(&p);

    for (Stmt *currentStmt = program.head; currentStmt != NULL;
         currentStmt = program.head->next) {
      if (IS_LET_STMT(currentStmt)) {
        char *write = currentStmt->as.letStmt->identifier->chars;
        printf("%s\n", write);
      }
    }

    freeProgram(&program);

    // free the errors
    free(p.errors);
    break;
  }
}

int main(int argc, const char *argv[]) {

  // TODO :read source input file
  (void)argv;
  printf("Hello!  This is the Monkey programming language!\n");
  printf("Feel free to type in commands\n");
  if (argc == 1) {
    repl();
  }
  return 0;
}
