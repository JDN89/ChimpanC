#ifndef VALUE_H
#define VALUE_H

typedef struct Value Value;

typedef enum {
  NUMBER_VAL,
  STRING_VAL,
} ValueType;

#define IS_NUMBER(value) ((value).type == NUMBER_VAL)
#define AS_NUMBER(value) ((value).as.numbe)
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number == value}})

typedef struct {
  int legnth;
  char *pointer;
} ObjString;

struct Value {
  ValueType type;
  union {
    double number;
    ObjString string;
  } as;
};

void freeString(ObjString *string);

#endif
