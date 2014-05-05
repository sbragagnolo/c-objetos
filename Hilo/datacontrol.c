#include "datacontrol.h"

void DCbloquear (struct DataControl* self, Collection* thMtxCol){
	self->mtx->lock (self->mtx, thMtxCol);
}
void DCdesbloquear (struct DataControl*self, Collection*thMtxCol) {
	self->mtx->unLock (self->mtx, thMtxCol);
}
void DCsetearEstado(struct DataControl*self, int iEstado) {
	self->iEstado = iEstado;
	self->cnd->avisar(self->cnd); 
}
int  DCesperarEstado(struct DataControl*self) {
	self->cnd->esperar(self->cnd, self->mtx);
	return self->iEstado;
}
void DCdestruir (struct DataControl*self, Collection*thMtxCol) {
	FREE (self->cnd);
	self->mtx->destruir (self->mtx, thMtxCol);
	FREE (self);
	
}


void (*__________destruirDC) (DataControl*,Collection*);
DataControl* DCnew (void) {
DataControl* nuevo;
String* nombre;
static int iDcSeq = 0;

		nuevo = CALLOC (1, sizeof (DataControl));
		nuevo->iEstado = -1;
		nuevo->cnd = ConditionClass.new ();
		nombre = StringClass.newCon ("DC: mem[%p] seq[%d]", nuevo, iDcSeq++);
		nuevo->mtx = MutexClass.new (nombre);
		nombre->destruir (nombre);
		nuevo->bloquear = DataControlClass.bloquear;
		nuevo->desbloquear = DataControlClass.desbloquear;
		nuevo->setearEstado = DataControlClass.setearEstado ;
		nuevo->esperarEstado = DataControlClass.esperarEstado ;
		nuevo->destruir = DataControlClass.destruir;
		
return nuevo;		
}

void DataControlClass_GoLive (void) {
__________destruirDC = DCdestruir;
		MutexClass_GoLive ();
		ConditionClass_GoLive ();
		StringClass_GoLive();
DataControlClass.bloquear = DCbloquear;
DataControlClass.desbloquear = DCdesbloquear;
DataControlClass.setearEstado = DCsetearEstado;
DataControlClass.esperarEstado = DCesperarEstado ;
DataControlClass.destruir = __________destruirDC;
DataControlClass.new = DCnew;

}





