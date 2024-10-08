# ChimpanC
C Implementation of 'Writing an interpreter in GO - Thorsten Ball'

## TODO

- unwind the errors in case of errors before exiting
- parse return statment
- write test
- simplify code and improve memeory usage:
    - Do I need to create a literal for all the tokenTypes? -> LET will always be let?
    - Do I even need to store a pointer? can't I just store the starting position of the thing in the source code and then callculate the offset of the start of the identifier, literal, number and the start of the next token?
        - int instead of pointer allready saves 4 bytes!




## Notes

### Lexer

Token value names get put put in the top namespace in clang. This caused an issue where TokenType EOF conflicted with EOF in <stdio.h>. The solution is prepending all the enum values, so they don't conflict with existing keywords in CLANG.

Go works with string slices to get a shallow copy to the source String(points to the original string). In go strings are immutable so the source String will never change. In Chimp, we point to the original source stream, this means that we must make sure that source string outlives the token literal and that the source string doesn't get changed as long as the token hasn't been processed!

The way we look for Keywords is different from the go version. In go we use a map and in ChimpanC we use a simple switch statement (minimalistic TRIE) [TRIE wiki](https://en.wikipedia.org/wiki/Trie). I got the idea from Clox. [Ch 16.4 Identifiers and keywords](https://craftinginterpreters.com/scanning-on-demand.html).

### Parser


