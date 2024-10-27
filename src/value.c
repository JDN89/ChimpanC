#include "value.h"

#include <stdio.h>
#include <stdlib.h>



void freeString(ObjString *obj) {
  if (obj == NULL) {
    return;
  }
  free(obj->pointer);
  free(obj);
}
