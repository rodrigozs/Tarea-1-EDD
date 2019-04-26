#pragma once
#include "../data/data.h"

/** Struct que se va a almacenar en el heap. Tiene key y value */
typedef struct object
{
  /** Key por el cual se ordena en el heap */
  double key;
  /** Puede ser cualquier cosa. la idea es hacer un heap que guarde structs */
  Vector* value;
} Object;

/** Inicializa el objeto */
Object* obj_init(double key, Vector* value);

/** Destruye el objeto */
void obj_destroy(Object* obj);
