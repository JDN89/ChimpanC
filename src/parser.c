#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "utilities.h"
#include "value.h"
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
  PREFIX,
  CALL
} Precedece;

typedef Expr *(*ParseFn)(Parser *p);
typedef Parser p;

static ParseFn *getPrefixRule(TokenType ttype); // Forward declaration
static Expr *createIdentifierExpr(Identifier *identifier);
static Expr *createNumberExpression(Value *number);
Expr *parseExpression(Parser *p, Precedece prec);

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

  // Read two tokens, so curToken and peekToken are both set
  advance(&p);
  advance(&p);
  p.errorCount = 0;
  return p;
};

bool isLineBreak(Parser *p) { return *p->ct.literal == '\n'; }

void consumeSemiColonAndNewline(Parser *p) {
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

void registerParserError(Parser *p, char *message) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "%s  -  TOKEN_TYPE: %s \n", message,
           tokenTypeToString(p->ct.type));
  p->errorCount++;
  p->errors[p->errorCount - 1] = msg;
}

void freeParserErrors(Parser *p) {
  for (int i = 0; i < +p->errorCount; i++) {
    if (p->errors[i] != NULL) {
      printf("%s", p->errors[i]);
      free(p->errors[i]);
      p->errors[i] = NULL;
    }
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

Expr *parseIdentifier(Parser *p) {

  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");

  Value *value = createStringValue(p->ct.length, p->ct.literal);

  identifier->ttype = TOKEN_IDENTIFIER;
  identifier->value = value;

  advance(p);

  return createIdentifierExpr(identifier);
}

Expr *createIdentifierExpr(Identifier *identifier) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed allocating memory for identifierLiteral \n.");
  expr->type = IDENTIFIER_EXPR;
  expr->as.identifier = identifier;
  return expr;
}

Expr *parseNumber(Parser *p) {

  // TODO: free IntegerLiteral
  NumberLiteral *number = malloc(sizeof(NumberLiteral));

  char *literal = malloc((char)p->ct.length + 1);
  memcpy(literal, p->ct.literal, p->ct.length);
  literal[p->ct.length] = '\0';

  if (literal == NULL) {
    printf("Error during allocation of number literal value on the stack. \n");
    free(literal);
  }

  char *endptr;

  // Convert the *char to a number. We first create a substring because the
  // literal stored in the Token points to the entire source string, starting
  // where the current number begins.
  number->value = strtol(literal, &endptr, 10);

  if (*endptr != '\0') {
    char *errorMessage = "Conversion error, non-numeric character found: %s\n";
    registerParserError(p, errorMessage);
  }

  number->ttype = p->ct.type;
  advance(p);
  consumeSemiColonAndNewline(p);

  return createNumberExpression(number);
}

Expr *createNumberExpression(NumberLiteral *number) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr, "Failed allocating memory for IntegerLiteral \n.");
  expr->as.numberLiteral = number;
  expr->type = NUMBER_EXPR;
  return expr;
}

Expr *parsePrefixExpression(Parser *p) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed to allocate Expr in parsePrefixExpression\n");

  PrefixExpr *pre = malloc(sizeof(PrefixExpr));
  HANDLE_ALLOC_FAILURE(
      expr, "Failed to allocate PrefixExpr in parsePrefixExpression\n");

  char *literal = malloc(sizeof(char) * p->ct.length + 1);
  HANDLE_ALLOC_FAILURE(expr, "Failed to allocate memory for operator literal "
                             "in parsePrefixExpression\n");
  memcpy(literal, p->ct.literal, p->ct.length);
  literal[p->ct.length] = '\0';

  pre->op = literal;
  pre->token = p->ct.type;

  advance(p);

  pre->right = parseExpression(p, PREFIX);
  expr->as.prefix = pre;
  expr->type = PREFIX_EXPR;

  return expr;
}

Expr *parseExpression(Parser *p, Precedece prec) {

  ParseFn prefixRule = *getPrefixRule(p->ct.type);
  Expr *leftExpr = prefixRule(p);

  return leftExpr;
}

// TODO read parseprecedence notes in book and comment
PrefixRule pr[] = {[TOKEN_IDENTIFIER] = {parseIdentifier, LOWEST},
                   [TOKEN_INT] = {parseNumber, LOWEST},
                   [TOKEN_BANG] = {parsePrefixExpression, LOWEST},
                   [TOKEN_MINUS] = {parsePrefixExpression, LOWEST}

};

static ParseFn *getPrefixRule(TokenType ttype) { return &pr[ttype].prefix; }

///////////////////////////
// Parse statements
// ///////////////////

LetStmt *parseLetStatement(Parser *p) {
  LetStmt *letStmt = malloc(sizeof(LetStmt));
  // check pt and consume ct
  if (!expectPeekToken(p, TOKEN_IDENTIFIER)) {
    // TODO: consume until ;  so we can continue parsing and reporting erros.
    // There was a chapter in Crafting Interpreters - Clox that discussed
    // somehting similar. Look it up.
  }

  letStmt->expr = parseIdentifier(p);

  if (!expectPeekToken(p, TOKEN_ASSIGN)) {
    // TODO: consume until ;  so we can continue parsing and reporting erros
  }

  while (p->ct.type != TOKEN_SEMICOLON) {
    advance(p);
  }

  consumeSemiColonAndNewline(p);
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

  consumeSemiColonAndNewline(p);
  return returnStatement;
}

ExprStatement *parseExpressionStatement(Parser *p) {
  ExprStatement *stmt = malloc(sizeof(ExprStatement));
  stmt->expr = malloc(sizeof(Expr));

  HANDLE_ALLOC_FAILURE(stmt->expr, "Failed to alocate memory for ExprStatement")

  Expr *expr = parseExpression(p, LOWEST);

  stmt->expr = expr;

  return stmt;
}

Stmt *parseStmt(Parser *p) {

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
  while (p->pt.type != TOKEN_EOF) {
    Stmt *stmt = parseStmt(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
