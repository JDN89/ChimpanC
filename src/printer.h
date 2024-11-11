#ifndef PRINTER_H
#define PRINTER_H

#include "ast.h"
#include "parser.h"
#include <stdio.h>

void print_statement(Stmt *stmt);
void print_errors(Parser *p);

#endif
