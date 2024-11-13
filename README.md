# ChimpanC
A C implementation based on "Writing an Interpreter in Go" by Thorsten Ball.

The goal is to also read "Writing a Compiler in Go", create a C version of the byteCode VM, compile Monkey's bytecode to WebAssembly (WASM), write a Snake game in Monkey, and possibly extend the language as needed. Finally, I want to run the WASM binary in the browser.

## TODO
- Identifiers can appear as both a stand alone expression statement x; TOKEN_IDENTIFIER TOKEN_SEMICOLON. or inside LetStatement: let x = 4; Parsing of letstatment gets called via switch -> parsing of identifier passes via ParseExpressionStatement which I had to refactor and create a new method -> parse_identifier_expression. Inside the Letstament struct we wrap the Identifier field. inside ExpressionStatement, the identifier is wrapped inside an Expr STruct!!! I partily refactored the code but the tests and printer will be broken now.
- start renaming methods and struct names. Use Pascal_Case and snake_case. I find this easier to read 
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

### Simplified Pratt parising call stack (raw brain dump)
Prat parsing: as long as the next operator in the expression is higher you keep recursivly calling parse_infix_expression with the allready parsed first operand as an argument. The first operand becomes the left node of the infxi expression and the right node is the result of calling parse_expression with the precedence of the current op as argument. In parse expression you parse the next operand and if there is another operator with a __higher__ precedence you recursivley call infix expression again ... once you return from the while loop and you sart unwinding the call stack the infix nodes get constructed with left op as the result from the allready parsed expression and the right op being the result of the recursive calls.

        +
       / \
      +   d
     / \
    a   *
       / \
      b   c



Input: a + b * c + d

Initial Call to parse_exp(p, LOWEST)

Left: a
Operator: +

call infix (left a, SUM)

Right: Result of parse_exp(p, SUM)
First Recursive Call to parse_exp(p, SUM)

Left: b
Operator: *

Call infix (left b, operator)

Right: Result of parse_exp(p, PRODUCT)
Second Recursive Call to parse_exp(p, PRODUCT)

Right Operand: c
Return: This call returns c, completing b * c
__Returning to First parse_exp(p, LOWEST)__ Call

Left: b * c (constructed as the subtree)
Operator: +
Call infix ( a + (b*c)), SUM)
Right: Result of parse_exp(p, SUM) to handle d
Final Call to parse_exp(p, SUM)

Right Operand: d
Return: This call returns d, completing the right operand for the second +
