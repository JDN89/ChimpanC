#ifndef lexer_h
#define lexer_h

typedef enum {
  TOKEN_ILLEGAL,
  EOF,
  // identifiers and literals
  IDENTIFIER,
  INT,
  // Operators
  ASSIGN,
  PLUS,
  // Delimiters
  COMMA,
  SEMICOLON,

  LPAREN,
  RPAREN,
  LBRACE,
  RBRACE,

  // keywords
  FUNCTION,
  LET
} TokenType;

// NOTE: We dont's store the literal of the LEXEME in the Token, instead we
// point to the start of the lexeme literal in the source code and include the
// lexem start and end. Like this we don't have to duplicate strings and avoid
// processing time. We have to make sure that life time of the source code
// superseeds the Token lifetime.
typedef struct {
  TokenType type;
  const char  *literal; // pointer to the literal value. Better to pass pointer to the
  int length;
} Token;

typedef struct {
  char *start;
  char *current;
} Lexer;

Lexer *init_lexer(char *source, Lexer *lexer);
Token nextToken(Lexer *lexer);

#endif
