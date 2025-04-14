#ifndef __MIHASH_H__
#define __MIHASH_H__

typedef void *(*CopyFunction)(void *data);
/** Retorna una copia fisica del data */
typedef int (*CompFunction)(void *data1, void *data2);
/** Retorna un entero negativo si data1 < data2, 0 si son iguales y un entero
 * positivo si data1 > data2  */
typedef void (*DestroyFunction)(void *data);
/** Libera la memoria alocada para el data */
typedef unsigned (*HashFunction)(void *data);
/** Retorna un entero sin signo para el data */

typedef struct _HashTable *HashTable;

#define OCCUPIED 1 
#define FREE 0 

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  char* data ;
  int state;
  //void* data
  /* GList data; */
} HashBox;

/**
 * Estructura principal que representa la tabla hash.
 */
struct _HashTable {
  HashBox *elems;
  unsigned numElems;
  unsigned capacity;
  CopyFunction copy;
  CompFunction comp;
  DestroyFunction destr;
  HashFunction hash;
};

/*
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
HashTable hashtable_create(unsigned capacity, CopyFunction copy,
                          CompFunction comp, DestroyFunction destroy,
                          HashFunction hash);

/*
 * Retorna el numero de elementos de la tabla.
 */
int hashtable_nelems(HashTable table);

/*
 * Retorna la capacidad de la tabla.
 */
int hashtable_capacity(HashTable table);

/*
 * Destruye la tabla.
 */
void hashtable_destroy(HashTable table);

/**
 * Inserta un data en la tabla, o lo reemplaza si ya se encontraba.
 */
void hashtable_insert(HashTable table, void *data);

/**
 * Retorna el data de la tabla que coincida con el data dado, o NULL si el data
 * buscado no se encuentra en la tabla.
 */
void *hashtable_search(HashTable table, void *data);

/**
 * Elimina el data de la tabla que coincida con el data dado.
 */
void hashtable_eliminate(HashTable table, void *data);

// Redimensiona la hash (duplica)
HashTable hashtable_redim(HashTable table);

//Retorna el factor de carga de la hash
double hash_charge_factor(HashTable table);

#endif /* __MIHASH_H__ */