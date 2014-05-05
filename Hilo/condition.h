#ifndef CONDITION_H_
#define CONDITION_H_
#include "mutex.h"
typedef struct Condicional{
		pthread_cond_t unCond;
		void (*esperar) (struct Condicional*, MutexObj*);
		void (*publicar) (struct Condicional*); /* lo publica para todos los hilos*/
		void (*avisar) (struct Condicional*); /*avisa a un hilo*/
		/**pthread_cond_signal*/
} Condicional;

typedef struct __conditClass{
		void (*esperar) (struct Condicional*, MutexObj*);
		void (*publicar) (struct Condicional*); /* lo publica para todos los hilos*/
		void (*avisar) (struct Condicional*); /*avisa a un hilo*/
		Condicional* (*new)(void);		
}__conditClass;


__conditClass ConditionClass;

void ConditionClass_GoLive (void);

#endif /*CONDITION_H_*/
