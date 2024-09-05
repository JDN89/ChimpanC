#include "debugger.h"
#include "lexer.h"
#include <pwd.h>
#include <stdio.h>

static void repl() {
  char line[1024];
  Lexer lexer;

  // Infinit loop for reading and precessing input
  for (;;) {
    printf(">> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
    init_lexer(line, &lexer);
    Token token = nextToken(&lexer);

    // Process and print each token until EOF
    while (token.type != TOKEN_EOF) {
      printf("Token: Type = %s\n", tokenTypeToString(token.type)
             );
      token = nextToken(&lexer); // Get the next token
    }
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
