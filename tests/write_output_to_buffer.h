#ifndef WRITE_OUTPUT_TO_BUFFER
#define WRITE_OUTPUT_TO_BUFFER

#include "../src/ast.h"
#include "../src/parser.h"
#include <stddef.h>

typedef struct {
  size_t capacity;
  size_t size;
  char data[256];
} Buffer;

Buffer *write_statement(Stmt *stmt);

void init_buffer(Buffer *buffer);
void reset_buffer(Buffer *buffer);
void append_to_buffer(Buffer *buffer, char *source);
void print_buffer(Buffer *buffer) ;
void write_statement_to_output(Buffer *buffer, Stmt *stmt) ;

#endif
