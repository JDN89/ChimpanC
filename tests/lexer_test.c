#include "../src/lexer.h"
#include <stdio.h>
#include <assert.h>


void test_init_lexer() {
    char source[] = "=";
    Lexer lexer;
    init_lexer(source, &lexer);

    assert(*lexer.start == *source);
    assert(*lexer.current == *source);
}

void test_nextToken() {
    char source[] = "=";
    Lexer lexer;
    init_lexer(source, &lexer);

    Token token = nextToken();
    assert(token.type == ASSIGN);
    assert(token.literal == source);
    assert(token.length == 1); // Assuming 'x' is a single character
}

int main() {
    test_init_lexer();
    test_nextToken();
    printf("All tests passed.\n");
    return 0;
}
