#ifndef LOGGERHEADER
#define LOGGERHEADER
#define MAXLONG 100
#include "../fileDescriptor/file.h"
#include "../String/string.h"
#include <stdio.h>
typedef enum {
	SIEMPRE_ABIERTO = 1,
	APERTURAxDEMANDA = 2
} MODO_USO;

typedef enum {
	MEMORIA = 0
} ERR;

typedef struct LogObj {
	FileObj* file;
	String* sPathFile;
	String* sOwner;
	void (*escribir) (struct LogObj*,const char* format,...);	
	void (*flushHeader) (struct LogObj*);
	void (*puts) (struct LogObj*,String* unaEntrada);
	void (*open) (struct LogObj*);
	void (*setPath)(struct LogObj*, String*);
	void (*setOwner)(struct LogObj*, String*);
	String* (*getPath)(struct LogObj*);
	String* (*getOwner)(struct LogObj*);	
	void (*close) (struct LogObj*);
	void (*destruir)(struct LogObj*);
}LogObj;

typedef struct ___LogClass___ {
	FileObj* file;
	String* sPathFile;
	void (*escribir) (struct LogObj*,const char* format,...);
	void (*flushHeader) (struct LogObj*);
	void (*puts) (struct LogObj*,String* unaEntrada);
	void (*open) (struct LogObj*);
	void (*setPath)(struct LogObj*, String*);
	String* (*getPath)(struct LogObj*);
	void (*setOwner)(struct LogObj*, String*);
	String* (*getOwner)(struct LogObj*);	
	void (*close) (struct LogObj*);
	void (*destruir)(struct LogObj*);
	LogObj* (*new) (void);
	LogObj* (*newCon) (const char* sPath, String* sOwner);
	LogObj* (*new_Con) (String* sPath, String* sOwner);
}___LogClass___;

___LogClass___ LogClass;

void LogClass_GoLive (void);







#endif
