#include "parser.h"
#include "lexer.h"

void getToken(Parser *p) {
  p->curToken = p->peekToken;
  p->peekToken = nextToken(p->l);
}

Parser p;
Parser *newParser(Parser *p, Lexer *l) {
  p->l = l;

  // NOTE:Read two tokens, so curToken and peekToken are both set
  getToken(p);
  getToken(p);
  return p;
}


