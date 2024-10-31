# ChimpanC
A C implementation based on "Writing an Interpreter in Go" by Thorsten Ball.

The goal is to also read "Writing a Compiler in Go", create a C version of the byteCode VM, compile Monkey's bytecode to WebAssembly (WASM), write a Snake game in Monkey, and possibly extend the language as needed. Finally, I want to run the WASM binary in the browser.

## TODO
- refactored ast -> now fix parser.c and related tests!
- add char* next to Objstring. Walk the linked list upon freeing and free all the allocated literals;
- call free() also in tests!
- use Valgrind to look for memory leaks
- replace MAKEFILE with [Cmake](https://cmake.org/getting-started/)
- clean up code after parser chapter before continuing to the interpreter part and see that all allocated objects are properly freed! See if there is a library tool that helps detecting unfreed memory.
- Read [tigerbeetle](https://github.com/tigerbeetle/tigerbeetle/blob/main/docs/TIGER_STYLE.md) -> Assert more in your code!
- Fuzzy test the lexer and parser and see where the program crashes. Pump random text into it!
    - [LibAFL](https://github.com/AFLplusplus/LibAFL)
    - [OSSFuzz](https://github.com/google/oss-fuzz)
- implement sanitizer tests! [google Sanitizers](https://github.com/google/sanitizers?tab=readme-ov-file)

### Refactoring ideas
- simplify code and improve memeory usage:
    - Do I need to create a literal for all the tokenTypes? -> LET will always be let?
    - Do I even need to store a pointer? can't I just store the starting position of the thing in the source code and then callculate the offset of the start of the identifier, literal, number and the start of the next token?
        - int instead of pointer allready saves 4 bytes!

### Compile
```Run the following command to produce ./chimp executable -> make ``` \
```Run the following command to produce ./test_parser executable -> make test_parser ``` 

### Sources
[Crafting Interpreters - Robert Nystrom](https://craftinginterpreters.com/) \
[Writing an Interpreter in Go](https://interpreterbook.com/)
