#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "utilities.h"
#include "value.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  LOWEST = 0,
  EQUALS,
  LESSGREATER,
  SUM,
  PRODUCT,
  PREFIX,
  CALL
} Precedece;

typedef Expr *(*ParseFn)(Parser *p);
typedef Expr *(*Parse_Infix_Fn)(Parser *p, Expr *expr);
typedef Parser p;

static ParseFn *get_prefix_rule(TokenType ttype);       // Forward declaration
static Parse_Infix_Fn *get_infix_rule(TokenType ttype); // Forward declaration
static uint8_t cur_precedence(Parser *p);
static Expr *create_identifier_expr(Identifier *identifier);
static Expr *create_number_expr(Value *number);
static Block_Statement *parse_block_statement(Parser *p);
static Stmt *parse_statement(Parser *p);
Expr *parse_exp(Parser *p, Precedece prec);

typedef struct {
  ParseFn prefix;
  Precedece precedence;
} Prefix_Rule;

typedef struct {
  Parse_Infix_Fn infix;
  Precedece precedence;
} Infix_Rule;

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
}

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

void register_parser_error(Parser *p, char *message) {
  char *msg = malloc(256 * sizeof(char));
  snprintf(msg, 256, "%s  -  TOKEN_TYPE: %s - LITERAL: %s \n", message,
           token_type_to_string(p->ct.type), p->ct.literal);
  p->errorCount++;
  p->errors[p->errorCount - 1] = msg;
}

void free_parser_errors(Parser *p) {
  for (int i = 0; i < +p->errorCount; i++) {
    if (p->errors[i] != NULL) {
      printf("%s", p->errors[i]);
      free(p->errors[i]);
      p->errors[i] = NULL;
    }
  }
}

// check ttype but don't consume the token;
bool ct_is(Parser *p, TokenType ttype) { return p->ct.type == ttype; }
bool pt_is(Parser *p, TokenType ttype) { return p->pt.type == ttype; }

// Check and consume the token
// TODO @Jan CLEANUP!!
bool check_and_consume_ct(Parser *p, TokenType ttype) {
  if (p->ct.type == ttype) {
    advance(p);
    return true;
  } else
    return false;
}

bool expect_peek_token(Parser *p, TokenType ttype) {
  if (p->pt.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, token_type_to_string(ttype), token_type_to_string(p->pt.type));
  return false;
}

bool expect_current_token(Parser *p, TokenType ttype) {
  if (p->ct.type == ttype) {
    advance(p);
    return true;
  } else
    peekError(p, token_type_to_string(ttype), token_type_to_string(p->pt.type));
  return false;
}

Identifier *parse_identifier(Parser *p) {

  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");

  Value *value = create_string_value(p->ct.length, p->ct.literal);

  identifier->value = value;

  advance(p);

  return identifier;
}

Expr *parse_identifier_expr(Parser *p) {

  Identifier *identifier = malloc(sizeof(Identifier));
  HANDLE_ALLOC_FAILURE(identifier,
                       "Failed allocating memory for Identifier \n.");

  Value *value = create_string_value(p->ct.length, p->ct.literal);

  identifier->value = value;

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
  numberExp = create_number_expr(value);

  assert(numberExp->as.value != NULL);
  return numberExp;
}

Expr *parse_boolean(Parser *p) {
  assert((p->ct.type == TOKEN_TRUE) | (p->ct.type == TOKEN_FALSE));

  Expr *bool_expression = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(bool_expression,
                       "Failed allocating memory for bool_expression \n.");

  bool_expression->type = BOOLEAN_EXPR;
  if (p->ct.type == TOKEN_TRUE) {
    Value *value = create_boolean_value(true);
    bool_expression->as.value = value;
    return bool_expression;
  }
  Value *value = create_boolean_value(false);
  bool_expression->as.value = value;
  return bool_expression;
}

Expr *parse_prefix_exp(Parser *p) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed to allocate Expr in parse_prefix_expression\n");

  Prefix_Expression *pre = malloc(sizeof(Prefix_Expression));
  HANDLE_ALLOC_FAILURE(
      expr, "Failed to allocate PrefixExpr in parse_prefix_expression\n");

  assert(p->ct.literal[0] == '!' || p->ct.literal[0] == '-');
  if (p->ct.type == TOKEN_MINUS) {
    pre->op = '-';
  } else {
    pre->op = '!';
  }

  advance(p);

  pre->right = parse_exp(p, PREFIX);
  expr->as.prefix = pre;
  expr->type = PREFIX_EXPR;

  return expr;
}

Expr *parse_infix_expression(Parser *p, Expr *left) {

  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr,
                       "Failed to allocate Expr in parsePrefixExpression\n");

  Infix_Expression *infix = malloc(sizeof(Infix_Expression));

  HANDLE_ALLOC_FAILURE(infix,
                       "Failed to allocate Expr in parsePrefixExpression\n");

  // TODO: think of the best way to get the current value if else branch for op
  // values seems good for now.
  if (p->ct.type == TOKEN_PLUS) {
    strcpy(infix->op, "+");
  } else if (p->ct.type == TOKEN_EQ) {
    strcpy(infix->op, "==");
  } else if (p->ct.type == TOKEN_NOT_EQ) {
    strcpy(infix->op, "!=");
  } else if (p->ct.type == TOKEN_ASSIGN) {
    strcpy(infix->op, "=");
  } else if (p->ct.type == TOKEN_MINUS) {
    strcpy(infix->op, "-");
  } else if (p->ct.type == TOKEN_BANG) {
    strcpy(infix->op, "!");
  } else if (p->ct.type == TOKEN_ASTERISK) {
    strcpy(infix->op, "*");
  } else if (p->ct.type == TOKEN_SLASH) {
    strcpy(infix->op, "/");
  } else if (p->ct.type == TOKEN_LT) {
    strcpy(infix->op, "<");
  } else if (p->ct.type == TOKEN_GT) {
    strcpy(infix->op, ">");
  } else {
    register_parser_error(p, "Operator not supported");
  }

  infix->left = left;
  Precedece pre = cur_precedence(p);
  advance(p);
  Expr *right = parse_exp(p, pre);
  infix->right = right;

  expr->type = INFIX_EXPR;
  expr->as.infix = infix;
  return expr;
}

Expr *parse_grouped_expression(Parser *p) {
  advance(p);

  Expr *expr = malloc(sizeof(Expr));
  assert(expr != NULL);

  expr = parse_exp(p, LOWEST);

  if (!expect_peek_token(p, TOKEN_RPAREN)) {
    register_parser_error(
        p, "Expected peek token to be } in parse grouped expression");
  }
  return expr;
}

Expr *parse_if_expression(Parser *p) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(expr, "Failed to allocate memory for if_expression\n");
  expr->type = IF_EXPR;
  if (!expect_peek_token(p, TOKEN_LPAREN)) {
    free(expr);
    return NULL;
  }

  advance(p);

  Expr *condition = parse_exp(p, LOWEST);
  if (!condition) { // Check for parsing failure
    free(expr);     // Clean up before returning
    return NULL;
  }

  If_Expression *if_expression = malloc(sizeof(If_Expression));
  HANDLE_ALLOC_FAILURE(if_expression,
                       "Failed to allocate memory for If_Expression\n");
  if_expression->condition = condition;
  expr->as.if_expression = if_expression;

  if (!expect_peek_token(p, TOKEN_RPAREN)) {
    return NULL;
  }
  if (!expect_peek_token(p, TOKEN_LBRACE)) {
    return NULL;
  }

  expr->as.if_expression->consequence = parse_block_statement(p);
  if (pt_is(p, TOKEN_ELSE)) {
    advance(p);
    if (!expect_peek_token(p, TOKEN_LBRACE)) {
      return NULL;
    }
    expr->as.if_expression->alternative = parse_block_statement(p);
  }
  return expr;
}

Parameters *parse_function_parameters(Parser *p) {

  Parameters *params = malloc(sizeof(Parameters));
  HANDLE_ALLOC_FAILURE(params, "Failed allocating for Parameters");
  create_function_parameters(params);
  if (pt_is(p, TOKEN_RPAREN)) {
    advance(p);
    return params;
  }
  advance(p);
  Identifier *param = parse_identifier(p);
  write_to_function_parameters(params, param);
  while (pt_is(p, TOKEN_COMMA)) {
    advance(p);
    advance(p);
    param = parse_identifier(p);
    if (!param) {
      free(param);
      free_params(params);
      return NULL;
    }
    write_to_function_parameters(params, param);
  }
  if (!expect_peek_token(p, TOKEN_RPAREN)) {
    free(params); // Clean up allocated memory.
    return NULL;
  }
  return params;
}

Expr *parse_function_literal_expression(Parser *p) {
  Expr *expr = malloc(sizeof(Expr));
  HANDLE_ALLOC_FAILURE(
      expr, "failed allocation memory for function_literal_expression");
  expr->type = FUNCTION_LITERAL_EXPR;

  if (!expect_peek_token(p, TOKEN_LPAREN)) {
    free(expr);
    return NULL;
  }

  Parameters *params = malloc(sizeof(Parameters));
  params = parse_function_parameters(p);
  if (params != NULL) {
    expr->as.fn->parameters = params;
  }

  if (!expect_peek_token(p, TOKEN_RBRACE)) {
    free(params);
    return NULL;
  }
  expr->as.fn->body = parse_block_statement(p);

  return expr;
}

Expr *parse_exp(Parser *p, Precedece prec) {

  ParseFn prefixRule = *get_prefix_rule(p->ct.type);
  if (prefixRule == NULL) {
    register_parser_error(p, "No prefix rule registered for ");
    return NULL;
  }

  Expr *leftExpr = prefixRule(p);

  while (p->pt.type != TOKEN_SEMICOLON && prec < peek_precedence(p)) {
    Parse_Infix_Fn infix_rule = *get_infix_rule(p->pt.type);
    if (infix_rule == NULL) {
      return leftExpr;
    }
    advance(p);
    leftExpr = infix_rule(p, leftExpr);
  }

  return leftExpr;
}

Prefix_Rule pr[] = {
    [TOKEN_IDENTIFIER] = {parse_identifier_expr, LOWEST},
    [TOKEN_INT] = {parse_number, LOWEST},
    [TOKEN_BANG] = {parse_prefix_exp, LOWEST},
    [TOKEN_MINUS] = {parse_prefix_exp, LOWEST},
    [TOKEN_TRUE] = {parse_boolean, LOWEST},
    [TOKEN_FALSE] = {parse_boolean, LOWEST},
    [TOKEN_LPAREN] = {parse_grouped_expression, LOWEST},
    [TOKEN_IF] = {parse_if_expression, LOWEST},
    [TOKEN_FUNCTION] = {parse_function_literal_expression, LOWEST}};

static ParseFn *get_prefix_rule(TokenType ttype) { return &pr[ttype].prefix; }

static Infix_Rule ir[] = {[TOKEN_EQ] = {parse_infix_expression, EQUALS},
                          [TOKEN_NOT_EQ] = {parse_infix_expression, EQUALS},
                          [TOKEN_LT] = {parse_infix_expression, LESSGREATER},
                          [TOKEN_GT] = {parse_infix_expression, LESSGREATER},
                          [TOKEN_PLUS] = {parse_infix_expression, SUM},
                          [TOKEN_MINUS] = {parse_infix_expression, SUM},
                          [TOKEN_SLASH] = {parse_infix_expression, PRODUCT},
                          [TOKEN_ASTERISK] = {parse_infix_expression, PRODUCT}};

static Parse_Infix_Fn *get_infix_rule(TokenType ttype) {
  return &ir[ttype].infix;
}

uint8_t peek_precedence(Parser *p) { return ir[p->pt.type].precedence; }
uint8_t cur_precedence(Parser *p) { return ir[p->ct.type].precedence; }

//------------------------------------------------------------------
// Parse statements
//------------------------------------------------------------------

Block_Statement *parse_block_statement(Parser *p) {

  Block_Statement *block = malloc(sizeof(Block_Statement));
  if (block == NULL) {
    HANDLE_ALLOC_FAILURE(block,
                         "Failed to allocate memory for block_statement\n");
  }
  create_block_statement(block);

  // :NOTE consume { token
  advance(p);
  while (!ct_is(p, TOKEN_RBRACE) && !ct_is(p, TOKEN_EOF)) {
    Stmt *statement = parse_statement(p);
    if (statement != NULL) {
      write_block_statement(block, statement);
    }

    // this is where I divert from MONKEY_lang. I only advance when I don't
    // encounter a }
    if (ct_is(p, TOKEN_RBRACE)) {
      break;
    } else {
      advance(p);
    }
  }

  return block;
}

Let_Statement *parse_let_statement(Parser *p) {
  Let_Statement *letStmt = malloc(sizeof(Let_Statement));

  assert(p->ct.type == TOKEN_LET);

  // Look at ttype and consume token
  if (!expect_peek_token(p, TOKEN_IDENTIFIER)) {
    // TODO: consume until ;  so we can continue parsing and reporting erros.
    // There was a chapter in Crafting Interpreters - Clox that discussed
    // somehting similar. Look it up.
  }

  letStmt->name = parse_identifier(p);

  if (!expect_current_token(p, TOKEN_ASSIGN)) {
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

  return letStmt;
}

Return_Statement *parse_return_statement(Parser *p) {
  Return_Statement *returnStatement = malloc(sizeof(Return_Statement));
  HANDLE_ALLOC_FAILURE(returnStatement,
                       "Memory allocation failed for ReturnStatement\n");

  returnStatement->expr = malloc(sizeof(Expr));
  expect_current_token(p, TOKEN_RETURN);

  assert(returnStatement != NULL);

  returnStatement->type = TOKEN_RETURN;
  if (p->ct.type != TOKEN_SEMICOLON) {
    returnStatement->expr = parse_exp(p, LOWEST);
  }

  if (p->pt.type == TOKEN_SEMICOLON) {
    advance(p);
  }

  consume_new_line_and_semi_colon(p);
  return returnStatement;
}

Expression_Statement *parse_expression_statement(Parser *p) {
  Expression_Statement *stmt = malloc(sizeof(Expression_Statement));
  stmt->expr = malloc(sizeof(Expr));

  HANDLE_ALLOC_FAILURE(stmt->expr, "Failed to alocate memory for ExprStatement")

  Expr *expr = parse_exp(p, LOWEST);
  if (p->pt.type == TOKEN_SEMICOLON) {
    advance(p);
  }

  stmt->expr = expr;

  consume_new_line_and_semi_colon(p);
  return stmt;
}

Stmt *parse_statement(Parser *p) {

  Stmt *stmt = malloc(sizeof(Stmt));

  HANDLE_ALLOC_FAILURE(stmt, "Failed to allocate memeory for Stmt. \n");

  switch (p->ct.type) {
  case TOKEN_LET: {
    Let_Statement *letStmt = parse_let_statement(p);
    if (letStmt != NULL) {
      stmt->type = LET_STATEMENT;
      stmt->as.letStmt = letStmt;
      if (p->pt.type == TOKEN_SEMICOLON) {
        advance(p);
      }
      consume_new_line_and_semi_colon(p);
    } else {
      free(stmt);
      return NULL;
    }
    break;
  }

  case TOKEN_RETURN: {
    assert(p->ct.type == TOKEN_RETURN);
    Return_Statement *returnStatement = parse_return_statement(p);
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
    Expression_Statement *exprStmt = parse_expression_statement(p);
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
    push_statement(&prog, stmt);
  }

  return prog;
}
