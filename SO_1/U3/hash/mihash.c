#include "mihash.h"
//#include "glist.h"
#include <assert.h>
#include <stdlib.h>

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  char* data;
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

HashTable hashtable_create(unsigned capacity, CopyFunction copy, CompFunction comp, DestroyFunction destr,HashFunction hash){
  HashTable table =malloc(sizeof(struct _HashTable));
  assert(table!=NULL);
  table->elems=malloc(sizeof(HashBox)*capacity);
  assert(table->elems!=NULL);
  for(int i=0;i<capacity;i++){
    //table->elems[i].data=NULL;
    table->elems[i].data=calloc(20,sizeof(char));
  }
  /* for(int i=0;i<capacity;i++){
    table->elems[i].data=glist_crear();
  } 
  */
  table->numElems=0;
  table->comp=comp;table->copy=copy;table->destr=destr;table->hash=hash;table->capacity=capacity;
  return table;
}
int hashtable_nelemsm(HashTable table){
  return table->numElems;
}
int hashtable_capacity(HashTable table){
  return table->capacity;
}

void hashtable_destroy(HashTable table){

  for(unsigned int idx=0;idx<table->capacity;idx++){
    if(table->elems[idx].data !=NULL)
      (table->destr(table->elems[idx].data));
  }

  free(table->elems);
  free(table);
}

double hash_charge_factor(HashTable table){return(table->numElems/table->capacity);}

HashTable hashtable_redim(HashTable table){
  unsigned int newdim=table->capacity*2;
  HashTable n_table=hashtable_create(newdim,table->copy,table->comp,table->destr,table->hash);
  for(int i=0;i<table->capacity;i++){
    void* data=table->elems[i].data;
    unsigned idx=n_table->hash(data) % n_table->capacity;
    hashtable_insert(n_table,data);
  }
  hashtable_destroy(table);
  return n_table;
}

void hashtable_insert(HashTable table, void* data){
  unsigned idx= table->hash(data)% table->capacity;

  //insertar dato si esta libre la casilla

  if(table->elems[idx].data==NULL){
    if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
    table->numElems++;
    table->elems[idx].data=table->copy(data);
    /* glist_agregar_final(table->elems[idx].data,table->copy(data)); */
    return;
  }

  //sobreescribir el dato si el mismo ya se encontraba en la tabla
  if(table->comp(table->elems[idx].data,data)==0){
    if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
    table->destr(table->elems[idx].data);
    table->elems[idx].data=table->copy(data);
    return;
  }
  /* if(table->comp(table->elems[idx].data->data, data)==0){
    table->destr(table->elems[idx].data->data);
    glist_agregar_final(table->elems[idx].data,table->copy(data)); 
    return;
  }
    */
  //en caso de colision no asemo na :()
  else{
    if((table->numElems/table->capacity)*1.0>0.7){table=hashtable_redim(table);}
    
    /* glist_agregar_final(table->elems[idx].data,data); */
    for(int a=0;a!=table->capacity;idx=(idx+1)%table->capacity){
      if(table->elems[idx].data==NULL){
        table->numElems++;
        table->elems[idx].data=table->copy(data);
        
      }
      a++;
    }
    return;
  }
}


void* hashtable_search(HashTable table,void* data){
  unsigned idx= table->hash(data) % table->capacity;
  if(table->elems[idx].data==NULL){
    return NULL;
  }
  /* if(glist_buscar(table->elems[idx].data,data,table->comp)==0){return data;} */
  int a=0;
  for(;a!=table->capacity;idx=(idx+1)%table->capacity){
    if(table->comp(table->elems[idx].data,data)==0){
      a=table->capacity-1;
    }
    a++;
  }
  if(a!=table->capacity){return table->elems[idx].data;}
  else{return NULL;}
}

void hashtable_eliminate(HashTable table,void* data){
  unsigned idx= table->hash(data)% table->capacity;

  if(table->elems[idx].data==NULL){return;}

  int a=0;
  for(;a!=table->capacity;idx=(idx+1)%table->capacity){
    if(table->comp(table->elems[idx].data,data)==0){
      a=table->capacity;
      table->destr(table->elems[idx].data);
    }
    a++;
  }
  /* if(glist_buscar(table->elems[idx].data,data,table->comp)==0){
    table->destr(table->elems[idx].data);
    table->numElems--;
    table->elems[idx].data=NULL;
  } */
  return;
}


int main(){
  return 0;
}