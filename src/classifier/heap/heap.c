#include <stdlib.h>
#include <stdio.h>
#include "heap.h"

////////////////////////////////////////////////////////////////////////////////
//                  Funciones utiles para no escribir tanto                   //
////////////////////////////////////////////////////////////////////////////////

/** Retorna la key del objeto de la posicion dada */
static double key(Heap* heap, int i)
{
  return heap -> array[i] -> key;
}

/** Hace swap de los elementos indicados */
static void swap(Heap* heap, int i1, int i2)
{
  Object* aux = heap -> array[i1];
  heap -> array[i1] = heap -> array[i2];
  heap -> array[i2] = aux;
}

////////////////////////////////////////////////////////////////////////////////
//             Funciones para conservar las propiedades del heap              //
////////////////////////////////////////////////////////////////////////////////

/** Mueve el elemento hacia arriba hasta que este en su posicion correcta */
static void sift_up(Heap* heap, int pos)
{
  // Si ya estoy en la raiz, termino
  if (!pos) return;

  // Posicion del padre (mis hijos estan en la posicion 2n+1)
  int father = (pos - 1) / 2;

  // Si el padre es menor hago swap y sigo iterando
  if (key(heap, father) < key(heap, pos))
  {
    printf("%f es menor que %f, por lo que los cambiamos\n",key(heap, father), key(heap, pos));
    swap(heap, pos, father);
    sift_up(heap, father);
  }
}

/** Mueve el elemento hacia abajo hasta que este en su posicion correcta */
static void sift_down(Heap* heap, int pos)
{
  // Posicion de los hijos
  int left = pos * 2 + 1;
  int right = left + 1;

  // Si estoy en una hoja termino
  if (heap -> count <= left) return;

  // Veo cual de los hijos es menor
  int highter;
  if (heap -> count == left || key(heap, left) < key(heap, right))
  {
    highter = right;
  }
  else
  {
    highter = left;
  }

  // Si la key actual es menor a la del hijo mayor, hago swap y sigo bajando
  if (key(heap, pos) < key(heap, highter))
  {
    swap(heap, pos, highter);
    sift_down(heap, highter);
  }
}

////////////////////////////////////////////////////////////////////////////////
//                 Funciones de uso publico del heap                          //
////////////////////////////////////////////////////////////////////////////////

/** Inicializa un heap vacio */
Heap* heap_init(int size)
{
  Heap* heap = malloc(sizeof(Heap));

  heap -> count = 0;
  heap -> size = size;
  heap -> array = calloc(heap -> size, sizeof(Object*));

  return heap;
}

/** Inserta un elemento en el heap */
void heap_insert(Heap* heap, Object* obj)
{
  // Inserto el elemento al final del heap
  heap -> array[heap -> count] = obj;
  heap -> count += 1;
  // Lo desplazo hasta su posicion correcta del heap
  sift_up(heap, heap -> count - 1);
}

/** Elimina la cabeza del heap y la retorna */
Object* heap_pop(Heap* heap)
{
  // Si el heap esta vacio retorno NULL
  if (!heap -> count) return NULL;

  // Obtengo la cabeza del heap
  Object* obj = heap -> array[0];
  // Disminuyo el contador de elementos en 1
  heap -> count -= 1;

  // Si quedan elementos, remplazo la cabeza por el ultimo elemento
  if (heap -> count > 0)
  {
    heap -> array[0] = heap -> array[heap -> count];
    heap -> array[heap -> count] = NULL;
    // Restauro la propiedad de heap
    sift_down(heap, 0);
  }
  else
  {
    // Si no quedan elementos, seteo la cabeza en NULL
    heap -> array[0] = NULL;
  }

  // Retorno el elemento que elimine
  return obj;
}

/** Destruye el heap */
void heap_destroy(Heap* heap)
{
  for(int k = 0; k < heap->count; k++){
    free(heap->array[k]->value);
    free(heap->array[k]);
  }
  free(heap -> array);
  free(heap);
}

/** Ordena un arreglo de objetos usando un heap */
void heapsort_1(Object** array, int size)
{
  Heap *heap = heap_init(size);
  for(int i = 0; i < size; i++){
    heap_insert(heap, array[i]);
  }

  for(int i = 0; i < size; i++){
    array[i] = heap_pop(heap);
  }
  heap_destroy(heap);
}
