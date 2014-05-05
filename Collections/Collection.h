#ifndef ___COLLECTION
#define ___COLLECTION
#include <stdlib.h>
#include "../memLib/memManager.h"

/**
TODO: 
	a- implementar bool en una biblioteca mas global.
	b- ver si no es necesario cambiar la implementacion de mensajes 
	   de collection
*/

/**
	Collections:
	La libreria de coleccion esta implementada como una clase 
	con una estructura de mensajeria separada, la razon de esto es que 
	la estructura de funciones este una sola vez en memoria reduciendo o 
	anulando la redundancia de datos en memoria. La contra es que el tipo de dato
	de self dentro de la estructura debe ser void*, obligando a peligrosos 
	casteos y dificulta la lectura de codigo al estar los metodos a mas 
	profundidad que los datos

*/
typedef enum {
	TRUE = 1,
	FALSE = 0
}bool;

typedef struct __ColNodo__ {
	struct __ColNodo__* sgte;
	struct __ColNodo__* prev;
	void*	Objeto;
}__ColNodo__;



typedef struct __ColMensajes__ {
	int (*add) (void* self, void* unObjeto);
	int (*addAll) (void* self, void* unaColeccion);
	bool (*allSatisfy) (void* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*anySatisfy) (void* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	void* (*collect) (void* self, void* (*elaborador) (void* unObjetoDeCol));
	bool (*detect) (void* self, bool (*Comparador) (void* self, void* unObjetoDeCol, void*unaClave), void* unaClave);
	void* (*inyect) (void* self, bool (*Into) (void*self, void*unObjetoDeCol, void*Rslt), void* Rslt);
	void* (*select) (void* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do) (void* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do_not) (void* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	int (*count)(void* self);
	int (*insertar) (void* self, __ColNodo__* unNodo);
	void* (*getFirst) (void* self);
	void* (*getLast) (void* self);
	void* (*getAnyOne) (void* self);
	void* (*getOne) (void* self, bool(*) (void*,void*), void*);
	void (*destruir) (void*);
	bool (*remove) (void*,void*,bool  (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave));
	bool (*subDo) (void* self, bool (*Algo) (void* self, __ColNodo__* unObjetoDeCol, void* unaClave), void* unaClave);
}__ColMensajes__;

typedef struct __classCol__ {
	void* (*new) (void);
	__ColMensajes__* instanceMethods;
}__classCol__;


typedef struct Collection {
		__ColMensajes__* Mensaje;
		struct __ColNodo__* Cabeza;
		struct __ColNodo__* Cola;
		struct __ColNodo__* Actual;
}Collection;

__classCol__ CollectionClass;

void colClassGoLive(void);
#define CollectionClass_New() CollectionClass.new()
#endif




