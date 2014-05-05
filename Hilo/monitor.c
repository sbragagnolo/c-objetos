#include "monitor.h"

void MONbloquear (struct Monitor* self, Collection* thMtxCol) {
	self->mtx->lock (self->mtx, thMtxCol); 
}

void MONdesbloquear (struct Monitor* self, Collection* thMtxCol) {
	self->mtx->unLock (self->mtx, thMtxCol);
}

void MONesperarEstado (struct Monitor*self, int iEstado) {
	if (iEstado > self->iEstados || iEstado < 0) {
		/**error*/
		return;
	}
	self->cnds[iEstado]->esperar (self->cnds[iEstado], self->mtx);
}

void MONsenializarEstado (struct Monitor*self, int iEstado, Collection* thMtxCol) {
	if (iEstado > self->iEstados || iEstado < 0) {
		/**error*/
		return;
	}
	
	self->mtx->unLock (self->mtx, thMtxCol);
	self->cnds [iEstado]->avisar (self->cnds [iEstado]);
return;
}

Monitor* MONnew (int iLong) {
Monitor* nuevo;
String* nombre;
static int mtxMonSeq = 0;
int i;
	nuevo = CALLOC (1, sizeof (Monitor));
	nuevo->cnds = CALLOC (iLong,sizeof (Condicional*));
	for (i = 0; i < iLong ; i++) {
		nuevo->cnds[i] = ConditionClass.new ();
	}
	nombre = StringClass.newCon ("Monitor mem[%p] seq[%d]",nuevo,mtxMonSeq++);
	
	nuevo->mtx = MutexClass.new (nombre);
	nombre->destruir(nombre);
	nuevo->bloquear = MonitorClass.bloquear;
	nuevo->desbloquear = MonitorClass.desbloquear;
	nuevo->esperarEstado = MonitorClass.esperarEstado;
	nuevo->senializarEstado = MonitorClass.senializarEstado;
	nuevo->destruir = MonitorClass.destruir;
	nuevo->iEstados = iLong;
	
return nuevo;

}
void MONdestruir (struct Monitor* self, Collection* thMtxCol) {
int i;

	self->mtx->destruir(self->mtx,thMtxCol);
	
	for (i = 0 ; i < self->iEstados ; i++ ){
		FREE (self->cnds [i]);
	}
	FREE (self);
}


void (*________destruirMonitor) (struct Monitor* , Collection* );

void MonitorClass_GoLive (void){
	
	MonitorClass.bloquear = MONbloquear;
	MonitorClass.desbloquear = MONdesbloquear;
	MonitorClass.esperarEstado = MONesperarEstado;
	MonitorClass.senializarEstado = MONsenializarEstado;
	________destruirMonitor = MONdestruir;
	MonitorClass.destruir = ________destruirMonitor;
	MonitorClass.new = MONnew;
}


