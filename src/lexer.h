#ifndef lexer_h
#define lexer_h

// NOTE: Clang drops enum value names in the top namespace. TokenType EOF would
// conflict with EOF defined in <stdio.h>. That's why we have to prepend them
// with Token_

#include <stddef.h>
typedef enum {
  TOKEN_ILLEGAL,
  TOKEN_EOF,
  // identifiers and literals
  TOKEN_IDENTIFIER,
  TOKEN_INT,
  // Operators
  TOKEN_EQ,
  TOKEN_NOT_EQ,
  TOKEN_ASSIGN,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_BANG,
  TOKEN_ASTERISK,
  TOKEN_SLASH,
  TOKEN_LT,
  TOKEN_GT,

  // Delimiters
  TOKEN_COMMA,
  TOKEN_SEMICOLON,

  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LBRACE,
  TOKEN_RBRACE,

  // keywords
  TOKEN_FUNCTION,
  TOKEN_LET,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_RETURN
} TokenType;

// NOTE: We dont's store the literal of the LEXEME in the Token, instead we
// point to the start of the lexeme literal in the source code and include the
// lexem start and end. Like this we don't have to duplicate strings and avoid
// processing time. We have to make sure that life time of the source code
// superseeds the Token lifetime.
  //TODO: Do i need pointer to literal value for all the tokentypes? in case of let + ,... can't I just keep passing the token type?
  // a pointer literal is standard 8 bytes.... 
  // Can't I just pass the start of point and calculate where the next token starts? and parse the literal like that instead of moving here pointers around
  // reference speech of andrew kelly when you reformat
  // how can I meassure the performance and document it
typedef struct {
  TokenType type;
  size_t  length;
  const char
      *literal; // pointer to the literal value. Better to pass pointer to the
} Token;

typedef struct {
  char *start;
  char *current;
} Lexer;

Lexer init_lexer(char *source);
Token nextToken(Lexer *lexer);

char *token_type_to_string(TokenType type);

#endif
