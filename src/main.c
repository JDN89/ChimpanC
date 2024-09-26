#include "ast.h"
#include "debugger.h"
#include "lexer.h"
#include "parser.h"
#include <pwd.h>
#include <stdio.h>

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
    size_t i = 0;
    Stmt *currentStmt = program.head;
    while (i < program.length && currentStmt != NULL) {
      char *write = currentStmt->as.letStmt->identifier->chars;
      printf("%s \n", write);
      if (program.head->next != NULL && IS_LET_STMT(currentStmt)) {
        currentStmt = program.head->next;
      } else {
        break;
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
