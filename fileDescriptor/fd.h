#ifndef FILEDESCRIPTORHEADER
#define FILEDESCRIPTORHEADER
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

#define ERRMSJ 255


typedef struct FdObj {
	int ifd;
	char* sUltimoMsj;
	void (*close) (struct FdObj*);
	int (*leer)(struct FdObj*, void*, int size);
	int (*escribir)(struct FdObj*, void*, int size);
	void (*destruir) (struct FdObj*);
	int  (*getFd) (struct FdObj*);
	void (*open) (struct FdObj*, char* sPath, int, mode_t );
	int (*connect) (struct FdObj*, unsigned int iIP, unsigned short shPuerto);
	int (*listen) (struct FdObj*, unsigned int iIP, unsigned short shPuerto, int iBackLog);
	struct FdObj* (*accept) (struct FdObj*);
} FdObj ;


typedef struct ___FdObjClass {
	void (*open) (struct FdObj*, char* sPath, int, mode_t );
	void (*close) (struct FdObj*);
	int (*leer)(struct FdObj*, void*, int size);
	int (*escribir)(struct FdObj*, void*, int size);
	int  (*getFd) (struct FdObj*);
	void (*destruir) (struct FdObj*);
	int (*connect) (struct FdObj*, unsigned int iIP, unsigned short shPuerto);
	int (*listen) (struct FdObj*, unsigned int iIP, unsigned short shPuerto, int iBackLog);
	struct FdObj* (*accept) (struct FdObj*);
	FdObj* (*newCon) (int iFd);
	FdObj* (*new) (void);
} ___FdObjClass;


___FdObjClass FdObjClass;
void FdObjClass_GoLive(void);
#endif

