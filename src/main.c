#include "ast.h"
#include "lexer.h"
#include "parser.h"
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
      switch (currentStmt->type) {

      case LET_STATEMENT: {
        char *write = (char *)currentStmt->as.letStmt->identifier->value;
        printf("%s\n", write);
        break;
      }
      case RETURN_STATEMENT:
        printf("Return statement %s \n",
               tokenTypeToString(currentStmt->as.returnStmt->type));
        break;
      case EXPR_STATEMENT: {
        char *write =
            (char *)currentStmt->as.exprStmt->expr->as.identifier->value;
        printf("%s\n", write);

        break;
      }
      }
    }

    freeProgram(&program);

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
