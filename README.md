# ChimpanC
A C implementation based on "Writing an Interpreter in Go" by Thorsten Ball.

The goal is to also read "Writing a Compiler in Go", create a C version of the byteCode VM, compile Monkey's bytecode to WebAssembly (WASM), write a Snake game in Monkey, and possibly extend the language as needed. Finally, I want to run the WASM binary in the browser.

## TODO

- Not freeing allocated objects makes me nervous. I should expand the free program function before the code becomes unamnageble. + check that everywhere you call exit you free the program.
- replace MAKEFILE with (Cmake)[https://cmake.org/getting-started/]
- clean up code after parser chapter before continuing to the interpreter part and see that all allocated objects are properly freed! See if there is a library tool that helps detecting unfreed memory.

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
