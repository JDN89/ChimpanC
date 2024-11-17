#ifndef HELPER_BUFFER
#define HELPER_BUFFER

#include <stddef.h>
#include "../src/ast.h"
#include "../src/parser.h"

typedef struct {
  size_t capacity;
  size_t size;
  char *data;
} Buffer;

Buffer* write_statement(Stmt *stmt);

void init_buffer(Buffer *buffer, size_t capacity);
void free_buffer(Buffer *buffer);
void append_to_buffer(Buffer *buffer, const char *source);

#endif
