# ChimpanC
C Implementation of 'Writing an interpreter in GO - Thorsten Ball'

## BUG
building test causes segmentation fault.
In the test, when we call next token, the previously initialized lexer get's reset and I think this is becasuse I use a global lexer in lexer.c. Remove the global Lexer and pass a pointer to it in the functions
