 #ifndef ___ERROBJ
#define ___ERROBJ
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../memLib/memManager.h"

typedef struct ErrID {
	int iErr;
	char* (*hashCode) (struct ErrID*);
	int (*getId) (struct ErrID*);
	void (*setId) (struct ErrID*, int);
}ErrID;

typedef struct ErrObj {
	ErrID idErr;
	void* extra;
	char* (*hashCode) (struct ErrObj*);
	int (*getId) (struct ErrObj*);
	void (*handler) (struct ErrObj*);
	void (*setExtra) (struct ErrObj*, void*);
	void (*setHandler) (struct ErrObj*, void (*) (struct ErrObj*));
	int (*hayHandler) (struct ErrObj*);
	void (*porDefecto) (struct ErrObj*);
	void (*destruir)  (struct ErrObj*);
}ErrObj;

typedef struct ___ErrObjClass {
	ErrObj* (*new) (int,void*,void(*)(ErrObj*),void(*)(ErrObj*));
}___ErrObjClass;

___ErrObjClass ErrObjClass;
void porDefecto (ErrObj* unError) ;
void ErrObjClass_GoLive (void) ;
#endif

