#include "value.h"

#include <stdio.h>
#include <stdlib.h>

void freeString(ObjString *string) {
  if (string == NULL) {
    return;
  }
  free(string);
}
