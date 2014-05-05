#include "errobj.h"

char* ERRIDhashCode (ErrID* self) {
char code[30 + 1];
char* sReturn;
	snprintf (code,30,"%d",self->iErr);
	sReturn = CALLOC (strlen (code), sizeof (char));
	strcpy (sReturn,code);
return sReturn;
}

int ERRIDgetId (ErrID* self) {
	return self->iErr;
}

void ERRIDsetId (ErrID* self, int id) { 
	self->iErr = id;
}


void porDefecto (ErrObj* unError) {
	printf ("\n\nError numero [%d] no tratado! Se cierra \n\n", unError->getId(unError));
	exit (unError->getId(unError));
}


char* ERROBJhashCode (struct ErrObj* self) {
	return self->idErr.hashCode (&(self->idErr));
}

int ERROBJgetId (struct ErrObj* self) {
	return self->idErr.getId (&(self->idErr));
}

void ERROBJsetId (struct ErrObj* self, int iID) {
	self->idErr.setId (&(self->idErr), iID);
}

void setPorDefecto (ErrObj* self, void (*porDefecto) (ErrObj*) ) {
	self->porDefecto = (porDefecto == NULL)? self->porDefecto:porDefecto;
return;
}



void ERROBJsetExtra (struct ErrObj* self, void* pExtra) {
	self->extra = pExtra;
return;
}

void ERROBJsetHandler (struct ErrObj* self, void (*handler) (struct ErrObj*)) {
	self->handler = handler;
}

int ERROBJhayHandler (struct ErrObj*self ) {
	return (self->handler != NULL);
}

void (*_______destruirERROBJ) (struct ErrObj*);

void destruirERROBJ (struct ErrObj* self) {
	FREE (self);
}

ErrObj* ERRCLASSnew (int iErr,void* pExtra,void(*handler)(ErrObj*),void(*porDefectoP)(ErrObj*)) {
ErrObj* unError;


	unError = CALLOC (1, sizeof (ErrObj));

	unError->idErr.hashCode = ERRIDhashCode;
	unError->idErr.setId = ERRIDsetId;
	unError->idErr.getId = ERRIDgetId;
	unError->getId = ERROBJgetId;
	unError->idErr.setId (&(unError->idErr), iErr);

	unError->hashCode = ERROBJhashCode;

	unError->handler = handler;
	unError->setHandler = ERROBJsetHandler;
	unError->porDefecto = (porDefectoP == NULL)? porDefecto:porDefectoP;
	unError->extra = pExtra;
	unError->setExtra = ERROBJsetExtra;
	unError->destruir = _______destruirERROBJ;
	unError->hayHandler = ERROBJhayHandler;
return unError;
	
}



void ErrObjClass_GoLive (void) {
	ErrObjClass.new = ERRCLASSnew;
	_______destruirERROBJ = destruirERROBJ;
}

