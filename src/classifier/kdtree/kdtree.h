#pragma once
#include <stdbool.h>
#include "../data/data.h"

/** Estructura de un kdtree */
typedef struct kdtree
{
    // Aca agrega los atributos que quieras para tu KD-Tree
    // eje es 0 si divide en el eje X y 1 si divide en el eje Y
    int eje = 1;
    kdtree* arriba_izq = NULL;
    kdtree* abajo_der = NULL;
    Data* vectores;

} KDTree;

/** Determina si el circulo definido por cx, cy, r choca con la caja */
bool collision(double cx, double cy, double r, double mx, double my, double Mx, double My);

/** Funcion que crea un kdtree a partir de los datos de entrenamiento */
KDTree* kd_init(Data* train);

/** Funcion que busca los k vecinos mas cercanos a objective
en el kd y los guarda en neighbours */
void knn(Vector** neighbours, KDTree* kd, Data* train_data, int k, Vector* objective);

/** Libera la memoria del kdtree */
void kd_destroy(KDTree* kd);
