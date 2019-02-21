#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TAMANIO_HEAP 10

struct heap{
	void** datos;
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
};

heap_t* heap_redimensionar(heap_t* heap,size_t tam_nuevo);

void swap (void** x, void** y){
	void* aux=*y;
	*y=*x;
	*x=aux;
}

void downheap(void *a[],cmp_func_t cmp, int n, int pos){
	if (pos>=n) return;
	int min=pos;
	int izq=2*pos+1;
	int der=2*pos+2;
	if (izq<n && cmp(a[izq],a[min])>0) min=izq;
	if (der<n && cmp(a[der],a[min])>0) min=der;
	if (min!=pos){
		swap(&a[min],&a[pos]);
		downheap(a,cmp,n,min);
	}
}

heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap=malloc(sizeof(heap_t));
	if (!heap) return NULL;
	void** datos=malloc(TAMANIO_HEAP*sizeof(void*));
	if (!datos) {free(heap); return NULL;}
	heap->datos=datos;
	heap->cant=0;
	heap->tam=TAMANIO_HEAP;
	heap->cmp=cmp;
	return heap;
}


void _heapify(heap_t* heap,size_t pos){
	if (pos>=heap->cant/2) return;
	_heapify(heap,pos*2+1);
	_heapify(heap,pos*2+2);
	downheap(heap->datos,heap->cmp,(int)heap->cant,(int)pos);	
}

void heapify(heap_t* heap){
	_heapify(heap,0);
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap=malloc(sizeof(heap_t));
	if (!heap) return NULL;
	void** datos=malloc((n+TAMANIO_HEAP)*sizeof(void*));
	if (!datos){ free(heap); return NULL;}
	for (int i=0;i<n;i++) datos[i]=arreglo[i];
	heap->datos=datos;
	heap->cant=n;
	heap->tam=n+TAMANIO_HEAP;
	heap->cmp=cmp;
	heapify(heap);
	return heap;
}

void upheap(void** datos, cmp_func_t cmp, int pos){
	if (pos==0) return;
	int padre=(pos-1)/2;
	if (cmp(datos[padre],datos[pos])<0){		
		swap(&datos[padre],&datos[pos]);
		upheap(datos,cmp,padre);
	}
}

bool heap_encolar(heap_t *heap, void *elem){
	if(!heap||!elem) return false;
	if(heap->tam==heap->cant) heap=heap_redimensionar(heap,heap->tam*2);
	if(!heap) return false;
    heap->datos[heap->cant]=elem;
	if (heap->cant>0) upheap(heap->datos,heap->cmp,(int)heap->cant);
    heap->cant++;
	return true;
}

void *heap_desencolar(heap_t *heap){
	if (!heap||heap->cant==0) return NULL;
	if(heap->cant*4<heap->tam&&heap->cant>TAMANIO_HEAP) heap=heap_redimensionar(heap,heap->tam/2);
	if(!heap) return NULL;
	void* elem=heap->datos[0];
	heap->datos[0]=heap->datos[heap->cant-1];
	downheap(heap->datos,heap->cmp,(int)heap->cant-1,0);
	heap->cant--;	
	heap->datos[heap->cant]=NULL;
	return elem;
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
	if (heap_cantidad(heap)==0) return true;
	return false;
}

void *heap_ver_max(const heap_t *heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if (destruir_elemento) for (int i=0;i<heap->cant;i++) destruir_elemento(heap->datos[i]);	
	free(heap->datos);
	free(heap);
}

heap_t* heap_redimensionar(heap_t* heap,size_t tam_nuevo){
	void* datos_nuevo = realloc(heap->datos, tam_nuevo*sizeof(void*));
    if (!datos_nuevo) return NULL;
	heap->datos=datos_nuevo;
    heap->tam=tam_nuevo;
	return heap;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heap_t* heap=heap_crear(cmp);
	if (!heap) return;
	for (size_t i=0;i<cant;i++){
		heap_encolar(heap,elementos[i]);
	}for (size_t i=0;i<cant;i++){
		elementos[cant-1-i]=heap_desencolar(heap);
	}
	heap_destruir(heap,NULL);	
}
