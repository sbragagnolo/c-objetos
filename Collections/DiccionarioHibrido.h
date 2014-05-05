#ifndef ___DICCIONARIOHIBRIDO
#define ___DICCIONARIOHIBRIDO
#include "Diccionario.h"
#include "Collection.h"
#include "../memLib/memManager.h"

/** TODO: puntos de decremento e implementacion de memoria en disminucion.*/

typedef enum {
	EN_USO = 1,
	BORRADO,
	REUTILIZADO
}MARCA;

/** 
	para permitir mayor flexibilidad el nodo a guardar en el hash y en la 
	coleccion es uno que no es propio a ninguno de los dos ni a lo que 
	quiera guardar al usuario, es un propio de la implementacion que permite
	sortear la problematica de actualizacion y borrado de datos de la tabla
	de hash basica. 
*/
typedef struct __DicNodo__ {
	char*	Tag;
	void*	Objeto;	
	char 	Marcado;
}__DicNodo__;


typedef struct DiccionarioHibrido {
	float pctIncremento;	/**	El tamaño de la tabla se agranda a X%*/
	int puntoIncremento;	/**   El tamaño de la tabla se agranda cuando llega a "p"*/
	int puntoDecremento;	/**   El tamaño de la tabla se achica cuando llega a "k"*/
	float pctRecontruccion; /**   La resconstruccion se da cuando se llega a g% de nodos borrados */
	int tamanioActual;	/**   La tabla mide n */
	int entradasHechas;	/**	El total de entradas hasta ahora es "e" */
	int entradasEliminadas; /**	Del total hay l entradas eliminadas */
	int entradasReales;	/**	Del total de entradas hay v vigentes */
	Collection* unaColeccion; /**	Soporte de coleccion */
	Diccionario* unDiccionario;	/** soporte de hash. */
/** diccionario path*/
	int (*insertar_keyTag) (struct DiccionarioHibrido*, char*, void* );
	int (*insertar_keyObj) (struct DiccionarioHibrido*, void*, void* );
	int (*insertar_Hasheable) (struct DiccionarioHibrido*, hasheable*, void* );
	int (*insertar) (struct DiccionarioHibrido*, char*, void* obj);
	void* (*buscar_keyTag) (struct DiccionarioHibrido*, char*);
	void* (*buscar_keyObj) (struct DiccionarioHibrido*, void*);
	void* (*buscar_Hasheable) (struct DiccionarioHibrido*, hasheable*);
	void* (*buscar) (struct DiccionarioHibrido*, char*);
/** collection path*/
	bool (*allSatisfy) (struct DiccionarioHibrido* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*anySatisfy) (struct DiccionarioHibrido* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	void* (*collect) (struct DiccionarioHibrido* self, void* (*elaborador) (void* unObjetoDeCol));
	bool (*detect) (struct DiccionarioHibrido* self, bool (*Comparador) (void*, void* unObjetoDeCol, void*unaClave), void* unaClave);
	void* (*inyect) (struct DiccionarioHibrido* self, bool (*Into) (void*self, void*unObjetoDeCol, void*Rslt), void* Rslt);
	void* (*select) (struct DiccionarioHibrido* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do) (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do_not) (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	int (*count)(struct DiccionarioHibrido* self);
/** hibrid path */
	void (*extender) (struct DiccionarioHibrido* self, int iNuevo);
	void (*reBuild) (struct DiccionarioHibrido* self);
	void (*setPctRebuild) (struct DiccionarioHibrido* self, int Pct);
	void (*setPuntoIncremento) (struct DiccionarioHibrido* self, int ptInc);
	void (*setPuntoDecremento) (struct DiccionarioHibrido* self, int ptDec);
	void (*setPctIncremento) (struct DiccionarioHibrido* self, int pctInc);
	void (*destruir) (struct DiccionarioHibrido* self);
	void (*remover_keyTag) (struct DiccionarioHibrido*, char*);
	void (*remover_keyObj) (struct DiccionarioHibrido*, void*);
	void (*remover_Hasheable) (struct DiccionarioHibrido*, hasheable*);
	void (*remover) (struct DiccionarioHibrido* self, char* tag);
}DiccionarioHibrido;





typedef struct HDCLass {
/** dicpath*/
	int (*insertar_keyTag) (struct DiccionarioHibrido*, char*, void* );
	int (*insertar_keyObj) (struct DiccionarioHibrido*, void*, void* );
	int (*insertar_Hasheable) (struct DiccionarioHibrido*, hasheable*, void* );
	int (*insertar) (struct DiccionarioHibrido*, char*, void* obj);
	void* (*buscar_keyTag) (struct DiccionarioHibrido*, char*);
	void* (*buscar_keyObj) (struct DiccionarioHibrido*, void*);
	void* (*buscar_Hasheable) (struct DiccionarioHibrido*, hasheable*);
	void* (*buscar) (struct DiccionarioHibrido*, char*);
/** colpath*/
	bool (*allSatisfy) (struct DiccionarioHibrido* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*anySatisfy) (struct DiccionarioHibrido* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	void* (*collect) (struct DiccionarioHibrido* self, void* (*elaborador) (void* unObjetoDeCol));
	bool (*detect) (struct DiccionarioHibrido* self, bool (*Comparador) (void*,void* unObjetoDeCol, void*unaClave), void* unaClave);
	void* (*inyect) (struct DiccionarioHibrido* self, bool (*Into) (void*self, void*unObjetoDeCol, void*Rslt), void* Rslt);
	void* (*select) (struct DiccionarioHibrido* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do) (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	bool (*Do_not) (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
	int (*count)(struct DiccionarioHibrido* self);
/** hibridpath */
	void (*extender) (struct DiccionarioHibrido* self, int iNuevo);
	void (*reBuild) (struct DiccionarioHibrido* self);
	void (*setPctRebuild) (struct DiccionarioHibrido* self, int Pct);
	void (*setPuntoIncremento) (struct DiccionarioHibrido* self, int ptInc);
	void (*setPuntoDecremento) (struct DiccionarioHibrido* self, int ptDec);
	void (*setPctIncremento) (struct DiccionarioHibrido* self, int pctInc);
	void (*destruir) (struct DiccionarioHibrido* self);
	void (*remover_keyTag) (struct DiccionarioHibrido*, char*);
	void (*remover_keyObj) (struct DiccionarioHibrido*, void*);
	void (*remover_Hasheable) (struct DiccionarioHibrido*, hasheable*);
	void (*remover) (struct DiccionarioHibrido* self, char* tag);
	struct DiccionarioHibrido* (*new) (struct HDCLass*, int tamanio);
}HDCLass;

HDCLass __DicHibridClass; 
void DicHibrid_GoLive (void);
#endif

