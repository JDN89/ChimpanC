#include "parser.h"
#include "lexer.h"
#include "dynstring.h"
#include "ast.h"

void getToken(Parser *p) {
  p->curToken = p->peekToken;
  p->peekToken = nextToken(p->l);
}

Parser newParser(Lexer *l) {
  Parser p;
  p.l = l;

  // NOTE:Read two tokens, so curToken and peekToken are both set
  getToken(&p);
  getToken(&p);
  return p;
}

