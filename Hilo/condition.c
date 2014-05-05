#include "condition.h"



void CONDesperar (Condicional* self, MutexObj* mtx) {
	pthread_cond_wait (&(self->unCond), &(mtx->unMutex));
}
void CONDpublicar (Condicional* self){
	pthread_cond_broadcast (&(self->unCond));
}

 /* lo publica para todos los hilos*/
void CONDavisar (Condicional* self){
	pthread_cond_signal (&(self->unCond));
}

 
Condicional* CONDnew (void ){
pthread_cond_t nn = PTHREAD_COND_INITIALIZER;
Condicional* nuevo;
	nuevo = CALLOC (1, sizeof (Condicional));
	nuevo->unCond = nn;
	nuevo->avisar = ConditionClass.avisar;
	nuevo->esperar = ConditionClass.esperar;
	nuevo->publicar = ConditionClass.publicar;
	
	return nuevo; 
}


void ConditionClass_GoLive (void) {
	ConditionClass.avisar = CONDavisar;
	ConditionClass.publicar = CONDpublicar;
	ConditionClass.esperar = CONDesperar;
	ConditionClass.new = CONDnew;

}





