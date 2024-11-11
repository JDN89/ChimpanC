#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include "ast.h"
#include "parser.h"

void print_statement(Stmt *stmt);
void print_errors(Parser *p);

#endif
