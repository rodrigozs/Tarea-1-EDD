#include <stdlib.h>
#include "object.h"

/** Inicializa el objeto */
Object* obj_init(double key, Vector* value)
{
  Object* obj = malloc(sizeof(Object));
  obj -> key = key;
  obj -> value = value;
  return obj;
}

/** Destruye el objeto */
void obj_destroy(Object* obj)
{
  free(obj);
}
