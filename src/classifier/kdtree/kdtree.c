#include <stdlib.h>
#include <math.h>
#include "kdtree.h"

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

int* Particion(Data* vectores, int i, int f, int eje){

    int low_idx = 0;
    int high_idx = 0;
    Vector lows[f-i];
    Vector highs[f-i];

    double p = vectores -> vectors[0] -> pos[eje];

    // Separo en Menores Iguales a p y Mayores a p
    for(int k = i; k <= f; k++){
        if(vectores -> vectors[k] -> pos[eje] <= p){
            lows[low_idx] = vectores -> vectors[k];
            low_idx = low_idx + 1;
        }
        else{
            highs[high_idx] = vectores -> vectors[k];
            high_idx = high_idx + 1;
        }
    }

    // Ordeno lista vectores
    for(k = 0; k < low_idx; k++){
        vectores -> vectors[k] = lows[k];
    }
    for(k = 0; k < high_idx; k++){
        vectores -> vectors[k] = highs[k];
    }

    return i + low_idx;

}

Vector Mediana(Data* vectores, int eje){
    int n = vectores -> count;
    int i = 0;
    int f = n-1;
    int* x = malloc(sizeof(int));
    int result;

    &x = Particion(vectores, i, f, eje);

    while(! &x == (n-1)/2 ){
        if(&x < (n-1)/2){
            i = &x + 1;
        }
        else{
            f = &x - 1;
        }
        &x = Particion(vectores, i, f, eje);
    }

    result = x;
    free(x);

    return vectores -> vectors[result] -> pos[eje];
}


/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree* kd_init(Data* train)
{
  // Completa este metodo

  // Creo una estructura KDTree
  KDTree* kd = malloc(sizeof(KDTree));

  // Agrego los vectores a kd
  kd -> vectores -> vectors = train -> vectors;
  kd -> vectores -> count = train -> count;

  // Si kd -> contador_vect <= 5 llegamos a la hoja
  if(&train -> count <= 5){
    return kd;
  }

  // Cambiamos el eje a dividir
  if(kd -> eje = 1){
    kd -> eje = 0;
  }
  else{
    kd -> eje = 1;
  }

  // Encuentro la mediana de los datos
  int mediana = Mediana(train, eje);

  // Creo 2 Datas para guardar los vectores de los hijos
  Data* arriba_izq;
  Data* abajo_der;
  int ai_idx = 0;
  int ad_idx = 0;


  // Creo 2 listas de menores-iguales y mayores a la mediana
  for(int k = 0; k < train -> count; k++){
    if(train -> vectors[k] -> pos[eje] <= mediana){
      arriba_izq -> vectors[ai_idx] = train -> vectors[k];
      ai_idx++;
    }
    else{
      kd_abajo_der -> vectors[ad_idx] = train -> vectors[k];
      ad_idx++;
    }
  }

  // Actualizamos el count de los Data
  arriba_izq -> count = ai_idx;
  abajo_der -> count = ad_idx;

  // Creamos los KDTrees hijos
  kd -> arriba_izq = kd_init(arriba_izq);
  kd -> abajo_der = kd_init(abajo_der);

  return kd;
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

  // Metodo naive (no eficiente)

  // Contador de vecinos cercanos
  int n_count = 0;

  // Itero por los datos de training
  for (int v = 0; v < train_data -> count; v++)
  {
    // Vector actual
    Vector* actual = train_data -> vectors[v];

    if (n_count < k)
    {
      // Si aun no he encontrado k vecinos, lo agrego al arreglo
      neighbours[n_count] = actual;
      n_count += 1;
    }
    else
    {
      /* Si entro aca es porque el arreglo esta lleno y debo buscar el vector
      del arreglo que esta a mayor distancia del objetivo */

      // Vector mas lejano y la distancia correspondiente
      int farest_index = -1;
      double max_dist = 0;

      // Busco el mas lejano
      for (int n = 0; n < k; n++)
      {
        double dist = distance(objective, neighbours[n]);
        if (dist >= max_dist)
        {
          farest_index = n;
          max_dist = dist;
        }
      }

      /* Una vez encontrado el vecino cercano mas lejano al objetivo, veo si
      el vector actual es mas cercano al objetivo */
      if (distance(objective, actual) < max_dist)
      {
        // Remplazo al vecino cercano mas lejano
        neighbours[farest_index] = actual;
      }
    }
  }
}

/** Libera la memoria del kdtree */
void kd_destroy(KDTree* kd)
{
  // Completa este metodo
  if(kd){
    kd_destroy(arriba_izq);
    kd_destroy(abajo_der);
    free(kd);
  }
}
