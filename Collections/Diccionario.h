#define _GNU_SOURCE
#define ___USE_GNU
#ifndef ___DICCIONARIO
#define ___DICCIONARIO



#include <search.h>
#include <string.h>
#include <stdlib.h>
#include "../memLib/memManager.h"

#define CODELONG 50
#define HASH_FAIL -1

typedef struct hasheable {
	char* (*hashString) (struct hasheable*);
	void* unObjeto;
}hasheable;
typedef struct hsearch_data hsearch_data;


typedef struct Diccionario {
	hsearch_data TablaHash;	
	int (*insertar_keyTag) (struct Diccionario*, char*, void* );
	int (*insertar_keyObj) (struct Diccionario*, void*, void* );
	int (*insertar_Hasheable) (struct Diccionario*, hasheable*, void* );
	int (*insertar) (struct Diccionario*, char*, void* obj);
	void* (*buscar_keyTag) (struct Diccionario*, char*);
	void* (*buscar_keyObj) (struct Diccionario*, void*);
	void* (*buscar_Hasheable) (struct Diccionario*, hasheable*);
	void* (*buscar) (struct Diccionario*, char*);
	void (*destruir) (struct Diccionario*);
}Diccionario;

typedef struct ClassDiccionario {
	int (*insertar_keyTag) (struct Diccionario*, char*, void* );
	int (*insertar_keyObj) (struct Diccionario*, void*, void* );
	int (*insertar_Hasheable) (struct Diccionario*, hasheable*, void* );
	int (*insertar) (struct Diccionario*, char*, void* obj);
	void* (*buscar_keyTag) (struct Diccionario*, char*);
	void* (*buscar_keyObj) (struct Diccionario*, void*);
	void* (*buscar_Hasheable) (struct Diccionario*, hasheable*);
	void* (*buscar) (struct Diccionario*, char*);
	void (*destruir) (struct Diccionario*);
	Diccionario* (*new) (struct ClassDiccionario*,int);

}ClassDiccionario;
ClassDiccionario DictionaryClass; /** clase .*/

void (*__Dic_destruir) (struct Diccionario*);
char* tagHashString (struct hasheable* self);
char* objHashString (struct hasheable* self);
void DiccionarioClass_GoLive (void);
#endif





