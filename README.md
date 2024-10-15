# ChimpanC
A C implementation based on "Writing an Interpreter in Go" by Thorsten Ball.

The goal is to also read "Writing a Compiler in Go", create a C version of the byteCode VM, compile Monkey's bytecode to WebAssembly (WASM), write a Snake game in Monkey, and possibly extend the language as needed. Finally, I want to run the WASM binary in the browser.

## TODO

- replace MAKEFILE with (Cmake)[https://cmake.org/getting-started/]
- write test for let, parse expression and return statement
- instead interrupting the program whilst parsing -> report the errors and keep parsing or should we intterupt during parsing? I think we should interupt during parsing, but not whilst interpreting?
- Now I interrupt the REPL while we encounter an EOF in parsing step, but I should create an infinte for loop and keep parsing and interpreting. (check book writing interperter in GO)

### Refactoring ideas
- simplify code and improve memeory usage:
    - Do I need to create a literal for all the tokenTypes? -> LET will always be let?
    - Do I even need to store a pointer? can't I just store the starting position of the thing in the source code and then callculate the offset of the start of the identifier, literal, number and the start of the next token?
        - int instead of pointer allready saves 4 bytes!

### Compile
run the following command to produce ./chimp executable -> make
run the following command to produce ./test_parser executable -> make test_parser
