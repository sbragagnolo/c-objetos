/**
	La implementaciòn del objeto 'ult' esta basada en los hilos pthread, estos hilos
son del tipo bloqueante, por lo que no son eficientes en multiples entradas salidas.
*/ 
#ifndef __HILOSULT
#define __HILOSULT
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "../memLib/memManager.h"

#define ULTSIZE sizeof (struct ___ult___)

struct ___ult___{
	pthread_t TID;
	void* (*fnAsociada) (void*);
	void* workSpace;
	void (*setFuncion) (struct ___ult___*, void*(*)(void*));
	void (*setWorkSpace) (struct ___ult___*, void*);
	void* (*getWorkSpace) (struct ___ult___*);
	int (*getTid) (struct ___ult___*);
	int (*detenerHilo)(struct ___ult___*);
	int (*iniciarHilo)(struct ___ult___*);
};

struct ___ultClass___{
	void (*setFuncion) (struct ___ult___*, void*(*)(void*));
	void (*setWorkSpace) (struct ___ult___*, void*);
	void*(*getWorkSpace) (struct ___ult___*);
	int (*getTid) (struct ___ult___*);
	int (*detenerHilo)(struct ___ult___*);
	int (*iniciarHilo)(struct ___ult___*);
	struct ___ult___* (*new) (void);
};


typedef struct ___ultClass___ ___ultClass___;
typedef struct ___ult___ ___ult___;
typedef struct ___ult___* ULThread;

void setFuncion (struct ___ult___*, void*(*)(void*));
void setWorkSpace (struct ___ult___*, void*);
void*getWorkSpace (struct ___ult___*);
int getTid (struct ___ult___*);
int iniciarHilo(struct ___ult___*);
struct ___ult___* ultNew (void);

struct ___ultClass___ ULThreadClass;
void ultClassGoLive (void);
#define ULThreadClass_new() ULThreadClass.new ()





#endif
