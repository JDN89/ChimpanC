#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "utilities.h"
#include "value.h"
#include <assert.h>
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

static ParseFn *get_prefix_rule(TokenType ttype); // Forward declaration
static Expr *create_identifier_expr(Identifier *identifier);
static Expr *create_number_expr(Value *number);
Expr *parse_exp(Parser *p, Precedece prec);

typedef struct {
  ParseFn prefix;
  Precedece precedence;
} PrefixRule;

void advance(Parser *p) {
  p->ct = p->pt;
  p->pt = nextToken(p->l);
}

Parser new_parser(Lexer *l) {
  Parser p = {0};
  p.l = l;

  // Read two tokens, so curToken and peekToken are both set
  advance(&p);
  advance(&p);
  /*p.errorCount = 0;*/
  return p;
};

bool isLineBreak(Parser *p) { return *p->ct.literal == '\n'; }

void consume_new_line_and_semi_colon(Parser *p) {
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

bool expect_peek_token(Parser *p, TokenType ttype) {
  if (p->pt.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, tokenTypeToString(ttype), tokenTypeToString(p->pt.type));
  return false;
}

bool expectCurrentToken(Parser *p, TokenType ttype) {
  if (p->ct.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, tokenTypeToString(ttype), tokenTypeToString(p->pt.type));
  return false;
}

Identifier *parse_identifier(Parser *p) {

  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");

  Value *value = createStringValue(p->ct.length, p->ct.literal);

  identifier->value = value;

  advance(p);

  return identifier;
}

Expr *parse_identifier_expr(Parser *p) {

  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");

  Value *value = createStringValue(p->ct.length, p->ct.literal);

  identifier->value = value;

  advance(p);

  return create_identifier_expr(identifier);
}

Expr *create_identifier_expr(Identifier *identifier) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed allocating memory for identifierLiteral \n.");
  expr->type = IDENTIFIER_EXPR;
  expr->as.identifier = identifier;
  return expr;
}

Expr *create_number_expr(Value *number) {
  assert(number != NULL); // Ensure number is not NULL
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr, "Failed allocating memory for IntegerLiteral \n.");
  expr->as.value = number;
  expr->type = NUMBER_EXPR;
  return expr;
}

Expr *parse_number(Parser *p) {
  assert(p->ct.type == TOKEN_INT);

  Expr *numberExp = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(numberExp,
                       "Failed allocating memory for identifierLiteral \n.");

  Value *value = createNumberValue(p->ct.literal);
  Expr *expr = create_number_expr(value);
  advance(p);
  consume_new_line_and_semi_colon(p);

  assert(expr->as.value != NULL);
  return expr;
}

Expr *parse_prefix_exp(Parser *p) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed to allocate Expr in parsePrefixExpression\n");

  PrefixExpr *pre = malloc(sizeof(PrefixExpr));
  HANDLE_ALLOC_FAILURE(
      expr, "Failed to allocate PrefixExpr in parsePrefixExpression\n");

  assert(p->ct.literal[0] == '!' || p->ct.literal[0] == '-');

  pre->op = p->ct.literal[0];

  advance(p);

  pre->right = parse_exp(p, PREFIX);
  expr->as.prefix = pre;
  expr->type = PREFIX_EXPR;

  return expr;
}

Expr *parse_exp(Parser *p, Precedece prec) {

  ParseFn prefixRule = *get_prefix_rule(p->ct.type);
  Expr *leftExpr = prefixRule(p);

  return leftExpr;
}

PrefixRule pr[] = {[TOKEN_IDENTIFIER] = {parse_identifier_expr, LOWEST},
                   [TOKEN_INT] = {parse_number, LOWEST},
                   [TOKEN_BANG] = {parse_prefix_exp, LOWEST},
                   [TOKEN_MINUS] = {parse_prefix_exp, LOWEST}

};

static ParseFn *get_prefix_rule(TokenType ttype) { return &pr[ttype].prefix; }

//------------------------------------------------------------------
// Parse statements
//------------------------------------------------------------------

LetStmt *parse_let_statement(Parser *p) {
  LetStmt *letStmt = malloc(sizeof(LetStmt));

  assert(p->ct.type == TOKEN_LET);

  // Look at ttype and consume token
  if (!expect_peek_token(p, TOKEN_IDENTIFIER)) {
    // TODO: consume until ;  so we can continue parsing and reporting erros.
    // There was a chapter in Crafting Interpreters - Clox that discussed
    // somehting similar. Look it up.
  }

  letStmt->name = parse_identifier(p);

  if (!expectCurrentToken(p, TOKEN_ASSIGN)) {
    // TODO: consume until ;  so we can continue parsing and reporting erros.
    // There was a chapter in Crafting Interpreters - Clox that discussed
    // somehting similar. Look it up.
    while (p->ct.type != TOKEN_SEMICOLON) {
      advance(p);
    }
  }

  else {
    Expr *expr = parse_exp(p, LOWEST);
    assert(expr != NULL);
    letStmt->expr = expr;
  }

  consume_new_line_and_semi_colon(p);
  return letStmt;
}

ReturnStatement *parse_return_statement(Parser *p) {
  ReturnStatement *returnStatement = malloc(sizeof(ReturnStatement));
  HANDLE_ALLOC_FAILURE(returnStatement,
                       "Memory allocation failed for ReturnStatement\n");

  expect_peek_token(p, TOKEN_RETURN);

  assert(returnStatement != NULL);

  returnStatement->type = TOKEN_RETURN;
  while (p->ct.type != TOKEN_SEMICOLON) {
    advance(p);
  }

  consume_new_line_and_semi_colon(p);
  return returnStatement;
}

ExprStatement *parse_expression_statement(Parser *p) {
  ExprStatement *stmt = malloc(sizeof(ExprStatement));
  stmt->expr = malloc(sizeof(Expr));

  HANDLE_ALLOC_FAILURE(stmt->expr, "Failed to alocate memory for ExprStatement")

  Expr *expr = parse_exp(p, LOWEST);

  stmt->expr = expr;

  consume_new_line_and_semi_colon(p);
  return stmt;
}

Stmt *parse_statement(Parser *p) {

  Stmt *stmt = malloc(sizeof(Stmt));

  HANDLE_ALLOC_FAILURE(stmt, "Failed to allocate memeory for Stmt. \n");

  switch (p->ct.type) {
  case TOKEN_LET: {
    LetStmt *letStmt = parse_let_statement(p);
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
    assert(p->ct.type == TOKEN_RETURN);
    ReturnStatement *returnStatement = parse_return_statement(p);
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
    ExprStatement *exprStmt = parse_expression_statement(p);
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
Program parse_program(Parser *p) {
  Program prog = createProgram();
  while (p->pt.type != TOKEN_EOF) {
    Stmt *stmt = parse_statement(p);
    pushtStmt(&prog, stmt);
  }

  return prog;
}
