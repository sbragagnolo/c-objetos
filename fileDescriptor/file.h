#ifndef FILEHEADER
#define FILEHEADER
#include "../errMan/errman.h"
#include "../memLib/memManager.h" 


#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <errno.h>


typedef struct FileObj {
	FILE* Flujo;
	void (*open) (struct FileObj*,char* sPath, char* modo);
	void (*close) (struct FileObj*);
	int (*leer)(struct FileObj*, void*, int size);
	int (*escribir)(struct FileObj*, void*, int size);
	int (*leerLinea)(struct FileObj*, void*, int size);
	void (*destruir) (struct FileObj*);
	int (*eof)( struct FileObj*);
	int (*error) (struct FileObj*);
} FileObj ;

typedef struct ___FileObjClass {
	void (*open) (struct FileObj*,char* sPath, char* modo);
	void (*close) (struct FileObj*);
	int (*leer)(struct FileObj*, void*, int size);
	int (*escribir)(struct FileObj*, void*, int size);
	int (*leerLinea)(struct FileObj*, void*, int size);
	void (*destruir) (struct FileObj*);
	int (*eof)( struct FileObj*);
	int (*error) (struct FileObj*);
	FileObj* (*new) (void);
	FileObj* (*newCon) (char*,char*);
} ___FileObjClass;

___FileObjClass FileObjClass;
void FileObjClass_GoLive (void);
#endif

