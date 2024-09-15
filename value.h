#include <stdbool.h>

// NOTE: how do we store and represent values? f.e. if you want to mulitply a
// number by true, you should get an error. Answer: tagged unions. The tag
// represents the type and the union represents the data. The union will be
// reserve the size of it's largest field; in our case now this is number.
// Update when using String

typedef enum { VAL_BOOL, VAL_NUMBER, VAL_OBJ } ValueType;

typedef enum {
  OBJ_STRING,
} ObjType;

typedef struct {
  ObjType type;
} Obj;

typedef struct {
  Obj obj;
  int length;
  char *chars;
} ObjString;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj *obj;
  } as;
} Value;

// NOTE: Convert C value to chimp value
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object) ((Value){VAL_OBJ, {.obj = (obj *)object}})

// NOTE: unwrap the stored stack value or pointer to the obj stored on the heap.
#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value) ((value).as.obj)
#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_C_STRING(value) ((ObjString *)AS_OBJ(value)->chars)

// NOTE: Type check before you access it in the above manner
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJ(value) ((value).type == VAL_OBJ)

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)

// NOTE: macro is expanded by inserting the argument expression every place the
// parameter name appears in the body. If a macro uses a parameter more than
// once, that expression gets evaluated multiple times.
static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString *copyString(const char *chars, int lenght);
