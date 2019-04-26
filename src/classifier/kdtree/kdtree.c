#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "kdtree.h"
#include "../heap/heap.h"


/*
En este archivo puedes agregar las funciones que quieras para tu codigo pero no
puedes modificar el input o el output de las funciones kd_init, knn y
kd_destroy
*/

/** Retorn true si el segmento de recta choca circulo */
static bool collision_line(double cx, double cy, double r, double x, double my, double My)
{
  /*
  Esta funcion es para ayudarlos, no es importante entender su funcionamiento
  */

  // Veo si la recta intersecta al circulo
  double det = pow(r, 2) - pow(cx - x, 2);
  if (det < 0) return false;

  // Calculo los puntos de interseccion
  double y0 = cy + sqrt(det);
  double y1 = cy - sqrt(det);

  // Veo si alguno esta entre my y My
  if ((my <= y0 && y0 <= My) || (my <= y1 && y1 <= My)) return true;

  // Retorno false
  return false;
}

/** Retorna true si el circulo definido por el heap toca al kdtree */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My)
{
  /*
  Esta funcion es para ayudarlos, no es importante entender su funcionamiento
  */

  // Caso 1: El centro del circulo esta dentro de la caja
  if (cx >= mx && cx <= Mx && cy >= my && cy <= My) return true;

  // Caso 2: El centro de la caja esta dentro del circulo
  // Calculo centro de la caja
  double bx = (Mx + mx) / 2;
  double by = (My + my) / 2;

  // Veo si esta en el circulo
  if (pow(bx - cx, 2) + pow(by - cy, 2) <= pow(r, 2)) return true;

  // Caso 3: Circunferencia choca con algun borde de la caja
  // Testeo con cada uno de los bordes
  if (collision_line(cx, cy, r, mx, my, My)) return true;
  if (collision_line(cx, cy, r, Mx, my, My)) return true;
  if (collision_line(cy, cx, r, my, mx, Mx)) return true;
  if (collision_line(cy, cx, r, My, mx, Mx)) return true;

  // Caso 5: No intersectan
  return false;
}

int cmpfunc(const void * a, const void * b){
  double num1 = *(double*)a;
  double num2 = *(double*)b;

  if(num1 < num2){
    return -1;
  }
  else if(num1==num2){
    return 0;
  }
  return 1;
}

double Mediana(Data* vectores, int eje){

    // Creo arreglo de datos
    double datos[vectores->count];

    for(int k= 0; k < vectores -> count; k++){
        datos[k] = vectores->vectors[k]->pos[eje];
    }
    
    // Ordeno con QuickSort
    qsort(datos, vectores->count, sizeof(double), cmpfunc);

    // Saco la mediana
    return datos[((vectores->count) -1 )/2];

}

void Print_KDTree(KDTree* kd){
  printf("count: %d\n",kd->vectores->count);
  printf("eje: %d\n", kd->eje);
  printf("min_x: %f - max_x: %f\n",kd->min_x, kd->max_x);
  printf("min_y: %f - max_y: %f\n",kd->min_y, kd->max_y);

  if(kd->vectores){
    printf("[");
    for(int k=0; k < kd->vectores->count; k++){
      printf("(%f, %f), ",kd->vectores->vectors[k]->pos[0], kd->vectores->vectors[k]->pos[1]);
    }
    printf("]\n");
    printf("\n");
  }
  
  if(kd->arriba_izq){
    printf("\nRama Izquierda\n");
    Print_KDTree(kd->arriba_izq);
  }

  if(kd->abajo_der){
    printf("\nRama Derecha\n");
    Print_KDTree(kd->abajo_der);
  }
}


KDTree* kd_init_aux(Data* train, int eje){
  // Creo una estructura KDTree
  KDTree* kd = malloc(sizeof(KDTree));
  kd->vectores = malloc(sizeof(Data));
  kd->vectores->vectors = malloc(sizeof(Vector*) * train -> count);

  // Agrego atributos a kd
  kd -> vectores -> vectors = train -> vectors;
  kd -> vectores -> count = train -> count;
  kd -> eje = eje;

  // Guardar el minimo y maximo
  //(1)double min_eje_x = kd->vectores->vectors[0]->pos[0];
  //(1)double max_eje_x = kd->vectores->vectors[0]->pos[0];
  //(1)double min_eje_y = kd->vectores->vectors[0]->pos[1];
  //(1)double max_eje_y = kd->vectores->vectors[0]->pos[1];

  double min_eje_x = train->vectors[0]->pos[0];
  double max_eje_x = train->vectors[0]->pos[0];
  double min_eje_y = train->vectors[0]->pos[1];
  double max_eje_y = train->vectors[0]->pos[1];

  // En X
  for(int k = 0; k < train->count; k++){
    //(1)if(kd->vectores->vectors[k]->pos[0] < min_eje_x){
      //(1)min_eje_x = kd->vectores->vectors[k]->pos[0];
    //(1)}
    //(1)if(kd->vectores->vectors[k]->pos[0] > max_eje_x){
      //(1)max_eje_x = kd->vectores->vectors[k]->pos[0];
    //(1)}
    if(train->vectors[k]->pos[0] < min_eje_x){
      min_eje_x = train->vectors[k]->pos[0];
    }
    if(train->vectors[k]->pos[0] > max_eje_x){
      max_eje_x = train->vectors[k]->pos[0];
    }
  }
  
  kd -> max_x = max_eje_x;
  kd -> min_x = min_eje_x;

  // En Y
  for(int k = 0; k < train->count; k++){
    //(1)if(kd->vectores->vectors[k]->pos[1] < min_eje_y){
      //(1)min_eje_y = kd->vectores->vectors[k]->pos[1];
    //(1)}
    //(1)if(kd->vectores->vectors[k]->pos[1] > max_eje_y){
      //(1)max_eje_y = kd->vectores->vectors[k]->pos[1];
    //(1)}

    if(train->vectors[k]->pos[1] < min_eje_y){
      min_eje_y = train->vectors[k]->pos[1];
    }
    if(train->vectors[k]->pos[1] > max_eje_y){
      max_eje_y = train->vectors[k]->pos[1];
    }
  }
  
  kd -> max_y = max_eje_y;
  kd -> min_y = min_eje_y;
  

  // Si kd -> contador_vect <= 5 llegamos a la hoja
  if(train -> count <= 5){
    return kd;

  }

  // Cambiamos el eje a dividir
  if(kd -> eje == 1){
    kd -> eje = 0;
    eje = 0;
  }
  else{
    kd -> eje = 1;
    eje = 1;
  }

  // Encuentro la mediana de los datos
  double mediana = Mediana(train, eje);
  kd -> mediana_xy = mediana;

  // Creo 2 Datas para guardar los vectores de los hijos
  Data* data_arriba_izq = malloc(sizeof(Data));
  Data* data_abajo_der = malloc(sizeof(Data));

  int ai_idx = 0;
  int ad_idx = 0;

  // Cuento cuantos elementos hay que ingresar a cada Data
  for(int k = 0; k < train -> count; k++){
    if(train -> vectors[k] -> pos[eje] <= mediana){
      ai_idx = ai_idx + 1;
    }
    else{
      ad_idx = ad_idx + 1;
    }
  }

  data_arriba_izq -> vectors = malloc(sizeof(Vector*) * ai_idx);
  data_abajo_der -> vectors = malloc(sizeof(Vector*) * ad_idx);

  // Creo 2 listas de menores-iguales y mayores a la mediana
  ai_idx = 0;
  ad_idx = 0;

  for(int k = 0; k < train -> count; k++){
    if(train -> vectors[k] -> pos[eje] <= mediana){
      data_arriba_izq -> vectors[ai_idx] = train -> vectors[k];
      ai_idx = ai_idx + 1;
    }
    else{
      data_abajo_der -> vectors[ad_idx] = train -> vectors[k];
      ad_idx = ad_idx + 1;
    }
  }

  // Actualizamos el count de los Data
  data_arriba_izq -> count = ai_idx;
  data_abajo_der -> count = ad_idx;


  // Creamos los KDTrees hijos
  kd -> arriba_izq = kd_init_aux(data_arriba_izq, kd -> eje);
  kd -> abajo_der = kd_init_aux(data_abajo_der, kd -> eje);

  return kd;
}


/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree* kd_init(Data* train)
{
  // Completa este metodo
  int eje = 1;
  KDTree* kd = kd_init_aux(train, eje);
  return kd;
  
}


/** Funcion para buscar sobre un KDtree*/
int SearchKDTree(KDTree* kd, Vector* vector, double* r, Heap* heap, int eje){

  if(!collision(vector->pos[0], vector->pos[1], *r, kd->min_x, kd->max_x, kd->min_y, kd->max_y)){
    printf("No hay colision \n\n");
    return 1;
  }
  
  //Si estamos en una hoja
  if(!kd->arriba_izq){
    printf("\n\nEstamos en una hoja \n");
    printf("Vector: (%f, %f)\n\n", vector->pos[0], vector->pos[1]);
    printf("Datos de la hoja:\n");
    printf("--> count vectores %d\n", kd->vectores->count);
    
    printf("--> [");
    for(int q=0; q < kd->vectores->count; q++){
      printf("(%f, %f), ", kd->vectores->vectors[q]->pos[0], kd->vectores->vectors[q]->pos[1]);
    }
    printf("]\n\n");

    // Itero sobre vectores de la particion
    for(int k=0; k < kd->vectores->count; k++){

      // Si hay espacio disponible en el heap
      if(heap->count < heap->size){
        printf("Hay espacio en mi heap Antes: (%d / %d) \n", heap->count, heap->size);

        Vector* vec_vector_heap = malloc(sizeof(Vector));
        vec_vector_heap = kd->vectores->vectors[k];

        Object* obj_vector = obj_init(distance(vector, kd->vectores->vectors[k]), vec_vector_heap);

        heap_insert(heap, obj_vector);

        printf("Hay espacio en mi heap Despues: (%d / %d) \n", heap->count, heap->size);

        printf("heap: [");
        for(int c=0; c < heap->count; c++){
          printf("(%f, %f), ", heap->array[c]->value->pos[0], heap->array[c]->value->pos[1]);
        }
        printf("]\n");

        printf("distancias: [");
        for(int c=0; c < heap->count; c++){
          printf("%f, ", heap->array[c]->key);
        }
        printf("]\n\n");
        
      }

      // Si mi heap esta lleno
      else if(heap->count == heap->size){
        printf("Mi heap esta lleno");
        for(int i=0; i < heap->count; i++){
          if(distance(kd->vectores->vectors[k], vector) < distance(heap->array[i]->value, vector)){

            Object* obj = heap_pop(heap);
            free(obj->value);
            free(obj);

            printf("heap: [");
            for(int c=0; c < heap->count; c++){
	      printf("(%f, %f), ", heap->array[c]->value->pos[0], heap->array[c]->value->pos[1]);
	    }
	    printf("]\n");

	    printf("distancias: [");
	    for(int c=0; c < heap->count; c++){
	      printf("%f, ", distance(vector, heap->array[c]->value));
	    }
	    printf("]\n\n");

            Object* obj_vector = malloc(sizeof(Object));
            obj_vector->key = heap->count;
            obj_vector->value = kd->vectores->vectors[k];

            heap_insert(heap, obj_vector);
          }
        }

        // Actualizo el radio
        double max = 0;
        for(int q=0; q < heap->count; q++){
          if(distance(vector,heap->array[q]->value) > max){
            max = distance(vector,heap->array[q]->value);
          }
        }
        *r = max;
      }

    }

    return 1;
  }

  else{
    if(vector->pos[kd->eje] < kd->mediana_xy){
      printf("%f < %f -> nos vamos por hijo arriba_izq\n",vector->pos[kd->eje], kd->mediana_xy);
      SearchKDTree(kd->arriba_izq, vector, r, heap, kd->eje);
      printf("Salgo de arriba_izq\n");
      SearchKDTree(kd->abajo_der, vector, r, heap, kd->eje);
    }

    else{
      printf("%f > %f -> nos vamos por hijo abajo_der\n",vector->pos[kd->eje], kd->mediana_xy);
      SearchKDTree(kd->abajo_der, vector, r, heap, kd->eje);
      printf("Salgo de arriba_izq\n");
      SearchKDTree(kd->arriba_izq, vector, r, heap, kd->eje);
    }
  }
  return 1;
}

/** Busca los vecinos cercanos y los guarda en neighbours */
void knn(Vector** neighbours, KDTree* kd, Data* train_data, int k, Vector* objective)
{
  /** Esta funcion es la que se encarga de realizar la busqueda de vecinos
  cercanos. Recibe como input neighbours (arreglo donde se deben guardar los
  vecinos cercanos encontrados), un kdtree (tu veras como lo usas), k (numero de
  vecinos a encontrar) y objective (el vector objetivo desde el cual se buscan
  los vecinos). Ademas tiene como input train_data para que funciones el metodo
  naive de knn. Tu puedes preservar el input de train_data o sacarlo.

  En este momento la funcion usa el metodo mas lento para entontrar los vecinos
  mas cercanos pero tu lo puedes optimizar usando un kdtree. Tu debes eliminar
  este codigo y mejorarlo pero se recomienda verlo primero para entenderlo.

  La idea de la funcion es que al terminar almacene en el vector neighbours a
  los k vecinos mas cercanos al vector objective. */

  // Metodo Eficiente
  double* r;
  double valor_r = 50000;
  r = &valor_r;

  // Creo un heap de tamanio size de objetos
  Heap* heap = heap_init(k);

  // Actualizo el heap con vecinos mas cercanos a objective
  SearchKDTree(kd, objective, r, heap, kd->eje);
  printf("Salgo de SearchKDTree");

  // Actualizo neighbours con los vecinos mas cercanos encontrados
  for(int i = 0; i < heap->size; i++){
    Object* obj = heap_pop(heap);
    neighbours[i] = obj->value;
  }
  
  // Destruimos heap
  heap_destroy(heap);



  // Metodo naive (no eficiente)

  // Contador de vecinos cercanos
  //int n_count = 0;

  // Itero por los datos de training
  //for (int v = 0; v < train_data -> count; v++)
  //{
    // Vector actual
    //Vector* actual = train_data -> vectors[v];

    //if (n_count < k)
    //{
      // Si aun no he encontrado k vecinos, lo agrego al arreglo
      //neighbours[n_count] = actual;
      //n_count += 1;
    //}
    //else
    //{
      /* Si entro aca es porque el arreglo esta lleno y debo buscar el vector
      del arreglo que esta a mayor distancia del objetivo */

      // Vector mas lejano y la distancia correspondiente
      //int farest_index = -1;
      //double max_dist = 0;

      // Busco el mas lejano
      //for (int n = 0; n < k; n++)
      //{
        //double dist = distance(objective, neighbours[n]);
        //if (dist >= max_dist)
        //{
          //farest_index = n;
          //max_dist = dist;
        //}
      //}

      /* Una vez encontrado el vecino cercano mas lejano al objetivo, veo si
      el vector actual es mas cercano al objetivo */
      //if (distance(objective, actual) < max_dist)
      //{
        // Remplazo al vecino cercano mas lejano
        //neighbours[farest_index] = actual;
      //}
    //}
  //}
}

/** Libera la memoria del kdtree */
void kd_destroy(KDTree* kd)
{
  // Completa este metodo
  if(kd->arriba_izq){
    kd_destroy(kd->arriba_izq);
  }
  if(kd->abajo_der){
    kd_destroy(kd->abajo_der);
  }

  free(kd->vectores->vectors);
  free(kd->vectores);
  free(kd);
}
