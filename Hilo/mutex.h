#ifndef MUTEX_H_
#define MUTEX_H_

#include "../errMan/errman.h"
#include <pthread.h>
#include "../Collections/Collection.h"
#include "../String/string.h"
#include "../Server/Server.h"
#include <errno.h>

typedef struct MutexObj {
	pthread_mutex_t unMutex;
	String* mutexName;
	void (*lock)(struct MutexObj*,Collection*);
	void (*Lock)(struct MutexObj*);
	bool (*tryLock)(struct MutexObj*);
	void (*unLock)(struct MutexObj*,Collection*);
	void (*destruir) (struct MutexObj*,Collection*);
	void (*setName) (struct MutexObj*, String*);
	char* (*getName) (struct MutexObj*);
}MutexObj;

typedef struct ___MutexClass {
	void (*lock)(struct MutexObj*,Collection*);
	void (*Lock)(struct MutexObj*);
	bool (*tryLock)(struct MutexObj*);
	void (*unLock)(struct MutexObj*,Collection*);
	void (*destruir) (struct MutexObj*,Collection*);
	void (*setName) (struct MutexObj*, String*);
	char* (*getName) (struct MutexObj*);
	MutexObj* (*new) (String*);/**PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP*/
	MutexObj* (*newCon) (char*);
}___MutexClass;


___MutexClass MutexClass;

void MutexClass_GoLive(void);






#endif /*MUTEX_H_*/
