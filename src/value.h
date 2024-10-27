#ifndef VALUE_H
#define VALUE_H

typedef struct Value Value;

typedef enum {
  VAL_NUMBER,
  VAL_STRING,
} ValueType;

#define IS_NUMBER(value) ((value).type == NUMBER_VAL)
#define AS_NUMBER(value) ((value).as.number)
#define NUMBER(value) ((Value){VAL_NUMBER, {.number = value}})

#define IS_STRING(value) ((value).type == STRING_VAL)
#define AS_STRING(value) ((value).as.string)
#define AS_CSTRING(value) ((value).as.string->pointer)

typedef struct {
  int length;
  char *pointer;
} ObjString;

struct Value {
  ValueType type;
  union {
    double number;
    ObjString *string;
  } as;
};

void freeString(ObjString *string);
ObjString *createString(int length, char *source);
Value *createStringValue( int length, char *source);

#endif
