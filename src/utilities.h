#define HANDLE_ALLOC_FAILURE(ptr, msg)                                         \
  if ((ptr) == NULL) {                                                         \
    fprintf(stderr, "%s\n", msg);                                              \
    free(ptr);                                                                 \
    exit(EXIT_FAILURE);                                                        \
}
