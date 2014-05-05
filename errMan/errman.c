#include "errman.h"

/**
	Hilo de manejo de señales de error. 
*/

void* SignalManager (ErrMan* self) {

ErrObj* unError;

	self->setSig (self, self->SenialError);

	while (1){
		self->esperarError (self);
		unError = self->errorOcurrido (self);
		
		if (unError == NULL) {
			printf ("El error invocado no existe en el directorio de errores! \n");
			exit (999);
		}
		if (unError->handler != NULL) unError->handler (unError);
		else unError->porDefecto (unError);
		self->ErrorOcurrido = NULL;
	}

}

/**
	inicializacion y ejecucuin del hilo. basado en el obj hilos ULThread (../hilos)
*/
int iniciar (struct ErrMan* self) {
	self->Manager->setFuncion (self->Manager, (void*(*)(void*))SignalManager);
	self->Manager->setWorkSpace(self->Manager, self);
	sigemptyset (&(self->SetBloqueo));
	sigaddset (&(self->SetBloqueo), self->SenialError);

	sigaddset (&(self->SetBloqueo), EMEM);

	return self->Manager->iniciarHilo(self->Manager);
}

int detener (struct ErrMan* self){ 
	printf ("detener Hilo no esta implementado (ERRMAN/ULThread)\n");
	return self->Manager->detenerHilo (self->Manager);
}


void setSig (struct ErrMan* self, int signo){
	if (signo == EMEM) {
		printf ("La señal para manejo de errores debe ser distinta a la de error fatal de memoria!! \n");
		exit (EMEM);
	}
	self->SenialError = signo;
}

int senialError(struct ErrMan* self) {
	return self->SenialError;
}

void esperarError (struct ErrMan* self) {
sigset_t Set;
int iSigno;
	Set = self->SetBloqueo;	
	sigwait(&Set, &iSigno);
	if (iSigno == EMEM && iSigno != self->SenialError) {
		printf ("Error fatal (asignaciòn de memoria) \n");
		exit (EMEM);
	}
return;
}

/** se da de alta un error, dependiendo si lo hace throw o catch se reciben distintos
 parametros*/

ErrObj* altaError (struct ErrMan* self, int iErr, void* pExtra,void(*handler)(ErrObj*), void(*porDefecto)(ErrObj*) ) {

ErrObj* unError;
	unError = ErrObjClass.new (iErr, pExtra, handler, porDefecto);
	self->Directorio->insertar (self->Directorio, unError->hashCode (unError), unError);

return unError;
}
/**catch, errid, handler. */

void ERRMANcatch (struct ErrMan* self, int iErr, void (*handler)(struct ErrObj*)) {
ErrObj* unError, *otro;

	otro = ErrObjClass.new (iErr, NULL, NULL, NULL);

	unError = self->Directorio->buscar (self->Directorio, otro->hashCode(otro));

	if (unError == NULL) {

		self->altaError (self, iErr, NULL, handler, NULL);
	}
	else {
		if (!(unError->hayHandler(unError)))		/* si no hay un handler, lo setea, si hay, pasa de largo. */
			unError->setHandler (unError,handler);	
	}
}

void ERRMANcatch_force (struct ErrMan* self, int iErr, void (*handler)(struct ErrObj*)) {
ErrObj* unError, *otro;

	otro = ErrObjClass.new (iErr, NULL, NULL, NULL);

	unError = self->Directorio->buscar (self->Directorio, otro->hashCode(otro));

	if (unError == NULL) {

		self->altaError (self, iErr, NULL, handler, NULL);
	}
	else {
			unError->setHandler (unError,handler);	
	}	
}

/** catch, errid, extra =D */

void ERRMANthrow (struct ErrMan* self, int iErr, void* Extra) {
ErrObj* unError, *otro;
	otro = ErrObjClass.new (iErr, NULL, NULL, NULL);

	unError = self->Directorio->buscar (self->Directorio, otro->hashCode(otro));
	if (unError != NULL) {
		unError->setExtra (unError,Extra);
		
	}
	else {
		unError = self->altaError (self, iErr, Extra, NULL, porDefecto);
	}

self->ErrorOcurrido = unError;
raise (self->senialError(self));
pthread_kill (self->Manager->getTid(self->Manager), self->senialError(self));
}

bool cleanerERRMAN_ENTRADAS (void* rSelf, void* unObjetoDeCol, void* unaClave) {
ErrObj* self = (ErrObj*) rSelf;
	self->destruir (self);
return TRUE;
}


void (*_________destuirERRMAN) (struct ErrMan*);
void destuirERRMAN (struct ErrMan* self) {
	self->Directorio->Do (self->Directorio, cleanerERRMAN_ENTRADAS, NULL);
	self->Directorio->destruir(self->Directorio);
	self->detener (self);
	FREE (self);
}

ErrMan* ERRMANCLASSnew (int signo) {
ErrMan* unManager;
	ErrObjClass_GoLive (); 
	StringClass_GoLive ();
	colClassGoLive ();
	DiccionarioClass_GoLive ();
	DicHibrid_GoLive ();
	ultClassGoLive ();
	unManager = CALLOC (1, sizeof(ErrMan));
	unManager->iniciar = ErrManClass.iniciar ;
	unManager->detener = ErrManClass.detener;
	unManager->setSig = ErrManClass.setSig ;
	unManager->senialError = ErrManClass.senialError ;
	unManager->esperarError = ErrManClass.esperarError ;
	unManager->errorOcurrido = ErrManClass.errorOcurrido ;
	unManager->altaError = ErrManClass.altaError ;
	unManager->catch = ErrManClass.catch ;
	unManager->catch_force = ErrManClass.catch_force ;
	unManager->throw = ErrManClass.throw;
	unManager->destruir = ErrManClass.destruir ;
	unManager->Manager = ULThreadClass_new();
	unManager->setSig (unManager, signo);
	unManager->Directorio = __DicHibridClass.new (&__DicHibridClass,20);
return unManager;
}

ErrObj* errorOcurrido (ErrMan* self) {
	return self->ErrorOcurrido;
}

void ErrManClass_GoLive (void) {

ErrManClass.iniciar = iniciar;
ErrManClass.detener = detener;
ErrManClass.setSig = setSig;
ErrManClass.senialError = senialError;
ErrManClass.esperarError = esperarError;
ErrManClass.errorOcurrido = errorOcurrido;
ErrManClass.altaError = altaError;
ErrManClass.catch = ERRMANcatch;
ErrManClass.catch_force = ERRMANcatch_force;
ErrManClass.throw = ERRMANthrow;
_________destuirERRMAN = destuirERRMAN;
ErrManClass.destruir = _________destuirERRMAN;
ErrManClass.new = ERRMANCLASSnew;

}


#ifdef ERRMANAGERSIGEXAMPLE
void handlerErr2 (ErrObj* unObj) {

	printf( "ERROR NUMERO 2, \"%s\"\n", unObj->extra);
	exit (2);
}

int main (void) {
ErrMan* Man;
sigset_t Set;

	sigemptyset (&Set);
	sigaddset (&Set, SIGINT);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;

ErrManClass_GoLive();
errManager = ErrManClass.new(SIGINT);
printf ("inicio de man %d \n",errManager->iniciar(errManager));



	printf ("ERRMAN Iniciado\n");
	catch(2, handlerErr2);
	printf ("Cachando error\n");

	throw (2, "HOLA MUNDO");

	printf ("chau\n");
usleep (1);
}

#endif


















 
