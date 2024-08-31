# ChimpanC
C Implementation of 'Writing an interpreter in GO - Thorsten Ball'

## Notes

Token value names get put put in the top namespace in clang. This caused an issue where TokenType EOF conflicted with EOF in <stdio.h>. The solution is prepending all the enum values, so they don't conflict with existing keywords in CLANG.
