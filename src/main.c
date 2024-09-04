#include <stdio.h>

#include "lexer.h"
#include <pwd.h>
#include <stdio.h>

static void repl(Lexer *lexer) {
  char line[1024];
  for (;;) {
    printf(">> ");
    if (!fgets(line, sizeof(line), stdin)) {
      printf("\n");
      break;
    }
  }
  init_lexer(line, lexer);
  Token token = nextToken(lexer);
  while (token.type != TOKEN_EOF) {
    printf("Token = %s", token.type);
  }
}

int main(int argc, const char *argv[]) {

  // TODO :read source input file
  (void)argv;
  printf("Hello!  This is the Monkey programming language!\n");
  printf("Feel free to type in commands\n");
  Lexer lexer;
  if (argc == 1) {
    repl(&lexer);
  }
  return 0;
}
