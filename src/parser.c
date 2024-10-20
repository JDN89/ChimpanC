#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HANDLE_ALLOC_FAILURE(ptr, msg)                                         \
  if ((ptr) == NULL) {                                                         \
    fprintf(stderr, "%s\n", msg);                                              \
    return NULL;                                                               \
  }

typedef enum {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIC,
  CALL
} Precedece;

typedef void *(*ParseFn)(Parser *p);
typedef Parser p;

typedef struct {
  ParseFn prefix;
  Precedece precedence;
} PrefixRule;

void advance(Parser *p) {
  p->ct = p->pt;
  p->pt = nextToken(p->l);
}

Parser newParser(Lexer *l) {
  Parser p;
  p.l = l;

  // NOTE:Read two tokens, so curToken and peekToken are both set
  advance(&p);
  advance(&p);
  p.errorCount = 0;
  return p;
};

bool isLineBreak(Parser *p) { return *p->ct.literal == '\n'; }

void consumeSemiColonAndLineBreak(Parser *p) {
  if (p->ct.type == TOKEN_SEMICOLON) {
    advance(p);
    if (isLineBreak(p))
      advance(p);
  }
}

void peekError(Parser *p, char *expected, char *got) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "Expected token %s, but got %s \n", expected, got);
  p->errorCount++;
  p->errors[p->errorCount - 1] = msg;
}

// TODO: test register parser error!
void registerParserError(Parser *p, char *message) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "%s  -  TOKEN_TYPE: %s \n", message,
           tokenTypeToString(p->ct.type));
  p->errorCount++;
  p->errors[p->errorCount - 1] = msg;
}

// TODO: Now we immediatley exit after calling this function -> better way to do
// this? Can't we just repost and keep parsing?
void freeParserErrors(Parser *p) {
  for (int i = 0; i < +p->errorCount; i++) {
    printf("%s", p->errors[i]);
    free(p->errors[i]);
    p->errors[i] = NULL;
  }
}

bool expectPeekToken(Parser *p, TokenType ttype) {
  if (p->pt.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, tokenTypeToString(ttype), tokenTypeToString(p->pt.type));
  return false;
}

bool expectCurrentToken(Parser *p, TokenType ttype) {
  if (p->pt.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, tokenTypeToString(ttype), tokenTypeToString(p->pt.type));
  return false;
}

void *parseIdentifier(Parser *p) {
  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");
  free(identifier);

  int length = p->ct.length;
  char *identifierLiteral = malloc(length + 1 * sizeof(char));

  HANDLE_ALLOC_FAILURE(identifierLiteral,
                       "Failed allocating memory for identifierLiteral \n.");
  memcpy(identifierLiteral, p->ct.literal, length);

  free(identifierLiteral);

  identifierLiteral[length] = '\0';

  identifier->length = length;
  identifier->ttype = TOKEN_IDENTIFIER;
  identifier->value = identifierLiteral;

  return (void *)identifier;
}

void *parseIntegerLiteral(Parser *p) {

  // TODO: free IntegerLiteral
  IntegerLiteral *integerLiteral = malloc(sizeof(IntegerLiteral));

  char *heapIntLit = malloc((char)p->ct.length + 1);
  memcpy(heapIntLit, p->ct.literal, p->ct.length);
  heapIntLit[p->ct.length] = '\0';

  if (heapIntLit == NULL) {
    printf("parsing of integer literal went wrong");
  }

  // convert intVal to int64_t
  char *endptr;

  integerLiteral->value = strtol(heapIntLit, &endptr, 10);

  if (*endptr != '\0') {
    char *errorMessage = "Conversion error, non-numeric character found: %s\n";
    registerParserError(p, errorMessage);
  }
  free(heapIntLit);

  integerLiteral->ttype = p->ct.type;
  consumeSemiColonAndLineBreak(p);

  return (void *)integerLiteral;
}

LetStmt *parseLetStatement(Parser *p) {
  LetStmt *letStmt = malloc(sizeof(LetStmt));
  // NOTE: check pt and consume ct
  if (!expectPeekToken(p, TOKEN_IDENTIFIER)) {
    freeParserErrors(p);
    exit(EXIT_FAILURE);
  }

  letStmt->identifier = parseIdentifier(p);

  if (!expectPeekToken(p, TOKEN_ASSIGN)) {
    freeParserErrors(p);
    exit(EXIT_FAILURE);
  }

  while (p->ct.type != TOKEN_SEMICOLON) {
    advance(p);
  }

  consumeSemiColonAndLineBreak(p);
  return letStmt;
}

ReturnStatement *parseReturnStatement(Parser *p) {
  ReturnStatement *returnStatement = malloc(sizeof(ReturnStatement));
  if (returnStatement == NULL) {

    fprintf(stderr, "Memory allocation failed for ReturnStatement\n");
  }
  advance(p);
  returnStatement->type = TOKEN_RETURN;
  while (p->ct.type != TOKEN_SEMICOLON) {
    advance(p);
  }

  consumeSemiColonAndLineBreak(p);
  return returnStatement;
}

PrefixRule pr[] = {[TOKEN_IDENTIFIER] = {parseIdentifier, LOWEST},
                   [TOKEN_INT] = {parseIntegerLiteral, LOWEST}};

static ParseFn *getPrefixRule(TokenType ttype) { return &pr[ttype].prefix; }

ExprStatement *parseExpressionStatement(Parser *p) {
  ExprStatement *stmt = malloc(sizeof(ExprStatement));
  stmt->expr = malloc(sizeof(Expr));
  if (stmt->expr == NULL) {
    HANDLE_ALLOC_FAILURE(stmt, "Failed to alocate memory for ExprStatement")
  }

  ParseFn prefixRule = *getPrefixRule(p->ct.type);

  switch (p->ct.type) {
  case TOKEN_IDENTIFIER: {

    Identifier *identifier = prefixRule(p);
    if (identifier->ttype == TOKEN_IDENTIFIER) {
      stmt->expr->as.identifier = identifier;
    }
    break;
  }
  case TOKEN_INT: {
    IntegerLiteral *intLit = prefixRule(p);

    if (intLit->ttype == TOKEN_INT) {
      stmt->expr->as.integerLiteral = intLit;
    }
    break;
  }

  default: {
    char *errorMessage = "Failed parsing ExpressionStatement for: ";

    registerParserError(p, errorMessage);
    free(stmt);

    return NULL;
  }
  }

  while (p->ct.type != TOKEN_SEMICOLON) {
    advance(p);
  }

  return stmt;
}

Stmt *parseStatement(Parser *p) {

  Stmt *stmt = malloc(sizeof(Stmt));

  HANDLE_ALLOC_FAILURE(stmt, "Failed to allocate memeory for Stmt. \n");

  switch (p->ct.type) {

  case TOKEN_LET: {
    LetStmt *letStmt = parseLetStatement(p);
    if (letStmt != NULL) {
      stmt->type = LET_STATEMENT;
      stmt->as.letStmt = letStmt;
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }
  case TOKEN_RETURN: {
    ReturnStatement *returnStatement = parseReturnStatement(p);
    if (returnStatement != NULL) {
      stmt->type = RETURN_STATEMENT;
      stmt->as.returnStmt = returnStatement;
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }
  default: {
    ExprStatement *exprStmt = parseExpressionStatement(p);
    if (stmt != NULL) {
      stmt->type = EXPR_STATEMENT;
      stmt->as.exprStmt = exprStmt;
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }
  }

  return stmt;
}

// TODO: convert program to a dynamic array of AST nodes
Program parseProgram(Parser *p) {
  Program prog = createProgram();
  // NOTE:  Stop looping when ct points to;
  while (p->pt.type != TOKEN_EOF) {
    Stmt *stmt = parseStatement(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
