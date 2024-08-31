#ifndef lexer_h
#define lexer_h

// NOTE: Clang drops enum value names in the top namespace. TokenType EOF would
// conflict with EOF defined in <stdio.h>. That's why we have to prepend them
// with Token_
typedef enum {
  TOKEN_ILLEGAL,
  TOKEN_EOF,
  // identifiers and literals
  TOKEN_IDENTIFIER,
  TOKEN_INT,
  // Operators
  TOKEN_ASSIGN,
  TOKEN_PLUS,
  // Delimiters
  TOKEN_COMMA,
  TOKEN_SEMICOLON,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  // keywords
  TOKEN_FUNCTION,
  TOKEN_LET
} TokenType;

// NOTE: We dont's store the literal of the LEXEME in the Token, instead we
// point to the start of the lexeme literal in the source code and include the
// lexem start and end. Like this we don't have to duplicate strings and avoid
// processing time. We have to make sure that life time of the source code
// superseeds the Token lifetime.
typedef struct {
  TokenType type;
  const char
      *literal; // pointer to the literal value. Better to pass pointer to the
  int length;
} Token;

typedef struct {
  char *start;
  char *current;
} Lexer;

Lexer *init_lexer(char *source, Lexer *lexer);
Token nextToken(Lexer *lexer);

#endif
