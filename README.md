# ChimpanC
A C implementation based on "Writing an Interpreter in Go" by Thorsten Ball.

## Compile and run

You need cmake version 3.28.3 at minimum.

```mkdir build && cd build``` \
```cmake ..``` \
```make``` \
```./chimp``` \
```make test``` \
```make test_lexer``` \
```make test_parser``` \
```make test_value``` \
```make clean```

## Sources
[Crafting Interpreters - Robert Nystrom](https://craftinginterpreters.com/) \
[Writing an Interpreter in Go](https://interpreterbook.com/)

## TODO

- fix bug parse block statement: 
```
(gdb) print p->ct.literal
$3 = 0x7fffffffe2bb "let t = 10; \n}else {y}" 
```
after calling parse block statement the let statments is not consumed at all? what gives? -> for tonight

- write down what you learned from fixing the bugs from yesterday
    - Advance tokens as much as possible in higher level functions. avoid manipulating the parser state in lower level functions as much as possible!
    - Previously, I advanced tokens inside lower-level parse functions like parse_identifier_expr. This caused unexpected behavior because advancing tokens too early disrupted the parsing flow at higher levels, making it difficult to maintain the proper order and context of token processing.
        - Token advancement should be done at logical boundaries in the parsing flow, typically at the statement or expression level, and only when the parser is certain the current token matches the expected pattern. Lower level functions shouldn't manipulate the parser state, because like this the overview is harder to maintain. when do we advance in which function and under which condition. this lead to to tight coupling -> take screenshot of git fix.
    - write down my allocation bug error. Parse function allocate memory inside the function but If_Exprs still had to be allocated
- implement @jan todo tasks
- start renaming methods and struct names. Use Pascal_Case and snake_case. I find this easier to read 
- call free() also in tests!
- Chimp aborts when it encounters a parsing error? shouldn't we keep parsing (so infinite loop until you end the program), at least as long you're in repl mode, and report the errors. Probably I am wrong but give some thought.
- use Valgrind to look for memory leaks
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


__First__ Recursive Call to parse_exp(p, SUM)  
Left: b  
Operator: *  

Call infix (left b, operator)  

Right: Result of parse_exp(p, PRODUCT)  


__Second__ Recursive Call to parse_exp(p, PRODUCT)  

Right Operand: c  
Return: This call returns c, completing b * c  
__Returning to First parse_exp(p, LOWEST)__ Call  

Left: b * c  
Operator: +  

Call infix ( a + (b*c)), SUM)  

Right: Result of parse_exp(p, SUM) to handle d  


__Final__ Call to parse_exp(p, SUM)  

Right Operand: d  
Return: This call returns d, completing the right operand for the second +  
