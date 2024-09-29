#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  LOWEST,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIC,
  CALL
} Precedece;

// NOTE: function pointer, that returns void * (Identifier,...)
// TODO:generalize return void * parseIdentifier,.. so we can reuse parseFn for
// the other parsing functions
typedef void *(*ParseFn)(Parser *p);
typedef Parser p;

typedef struct {
  ParseFn prefix;
  Precedece precedence;
} PrefixRule;

// TODO: convert program to a dynamic array of AST nodes
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

void peekError(Parser *p, char *expected, char *got) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "Expected token %s, but got %s \n", expected, got);
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

// TODO: change identifier to look at current token and create expect current
// token function
void *parseIdentifier(Parser *p) {
  Identifier *identifier = malloc(sizeof(Identifier));
  int length = p->ct.length;
  const char *source = p->ct.literal;
  if (identifier == NULL) {
    fprintf(stderr, "Failed to allocate memory for Identifier struct. \n");
    exit(EXIT_FAILURE);
  }
  char *heapchars = malloc(length + 1);

  if (heapchars == NULL) {
    fprintf(stderr, "Failed to allocate memory heapchars. \n");
    exit(EXIT_FAILURE);
  }
  memcpy(heapchars, source, length);
  heapchars[length] = '\0';

  identifier->length = length;
  identifier->ttype = TOKEN_IDENTIFIER;
  identifier->value = heapchars;

  return (void *)identifier;
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
  return returnStatement;
}

PrefixRule pr[] = {[TOKEN_IDENTIFIER] = {parseIdentifier, LOWEST}};

static ParseFn *getPrefixRule(TokenType ttype) { return &pr[ttype].prefix; }

ExprStatement *parseExpressionStatement(Parser *p) {
  ExprStatement *expr = malloc(sizeof(ExprStatement));

  // TODO: we probably have to switch on ct.type in next steps
  ParseFn prefixRule = *getPrefixRule(p->ct.type);

  Identifier *identifier = prefixRule(p);
  if (identifier == NULL) {
    fprintf(stderr, "Failed to parse an identifier.\n");
    exit(EXIT_FAILURE);
  }
  if (identifier->ttype == TOKEN_IDENTIFIER) {
    expr->expr->as.identifier = identifier;
  }
  return expr;
}

Stmt *parseStatement(Parser *p) {

  Stmt *stmt = malloc(sizeof(Stmt));

  if (stmt == NULL) {
    fprintf(stderr, "Memory allocation failed for Stmt\n");
    exit(EXIT_FAILURE);
  }

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
  default:
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

  return stmt;
}

Program parseProgram(Parser *p) {
  Program prog = createProgram();
  // NOTE:  Stop looping when ct points to;
  while (p->pt.type != TOKEN_EOF) {
    Stmt *stmt = parseStatement(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
