#ifndef VALUE_H
#define VALUE_H

#include <stdbool.h>
#include <stddef.h>
typedef struct Value Value;

typedef enum {
  VAL_NUMBER,
  VAL_STRING,
  VAL_BOOL,
} ValueType;

#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define AS_NUMBER(value) ((value).as.number)
#define NUMBER(value) ((Value){VAL_NUMBER, {.number = value}})

#define IS_BOOLEAN(value) ((value).type == VAL_BOOL)
#define AS_BOOLEAN(value) ((value).as.boolean)
#define BOOLEAN(value) ((Value){VAL_BOOL, {.boolean = value}})

#define IS_STRING(value) ((value).type == VAL_STRING)
#define AS_STRING(value) ((value).as.string)
#define AS_CSTRING(value) ((value).as.string->pointer)

typedef struct {
  size_t length;
  char *pointer;
} ObjString;

struct Value {
  ValueType type;
  union {
    bool boolean;
    double number;
    ObjString *string;
  } as;
};

Value *create_string_value(size_t length, const char *source);
void freeValue(Value *val);
Value *createNumberValue(const char *source);

#endif
