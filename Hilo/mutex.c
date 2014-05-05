#include "mutex.h"


bool colLocker (void* pSelf, void* objMtx, void* pDestino){

Collection* self = (Collection*)pSelf;
MutexObj* mtx = (MutexObj*)objMtx;
Collection* destino = (Collection*)pDestino;
printf ("..");
		mtx->lock (mtx,destino);
		self->Mensaje->remove(self, mtx, NULL);

return TRUE;
}

bool colUnLocker (void* pSelf, void* objMtx, void* pDestino){

Collection* self = (Collection*)pSelf;
MutexObj* mtx = (MutexObj*)objMtx;
Collection* destino = (Collection*)pDestino;
		printf ("||");
		mtx->unLock (mtx, self);


	destino->Mensaje->add(destino, mtx);

return TRUE;
}


void MTXlock(struct MutexObj* self,Collection* ThCol) {
Collection* bkp;

	if (self->tryLock(self) == TRUE) {
		ThCol->Mensaje->add(ThCol, self);
		return;
	}

	bkp = CollectionClass_New ();

	ThCol->Mensaje->Do (ThCol,colUnLocker,bkp);

	self->Lock (self);

	ThCol->Mensaje->add (ThCol,self);
	bkp->Mensaje->Do(bkp,colLocker,ThCol);
	bkp->Mensaje->destruir(bkp);

return;
	
}



bool MTXtryLock(struct MutexObj* self){

	switch (pthread_mutex_trylock (&(self->unMutex))) {
		case EINVAL:
			throw (EBADPARAM_TH, 
			StringClass.newCon("Error en bloqueo del mtx %s por el hilo %d",
			self->getName(self),
			pthread_self()));
			exit(EBADPARAM_TH);	/** <--- SALIDA OBLIGATORIA*/
			break;
		case EBUSY:
			return FALSE;
		case EDEADLK:
			throw (EDEADLOCK_TH, 
			StringClass.newCon("Error en bloqueo: el hilo %d quizo bloquear %s por segunda vez",
			pthread_self(),
			self->getName(self)));
			exit(EDEADLOCK_TH);	/** <--- SALIDA OBLIGATORIA*/
		default:
			return TRUE;
	}


}
void MTXLock_r(struct MutexObj* self) {
	switch (pthread_mutex_lock (&(self->unMutex))) {
		case EINVAL:
			throw (EBADPARAM_TH, 
			StringClass.newCon("Error en bloqueo del mtx %s por el hilo %d",
			self->getName(self),
			pthread_self()));
			exit(EBADPARAM_TH);	/** <--- SALIDA OBLIGATORIA*/

		case EDEADLK:
			throw (EDEADLOCK_TH, 
			StringClass.newCon("Error en bloqueo: el hilo %d quizo bloquear %s por segunda vez",
			pthread_self(),
			self->getName(self)));
			exit(EDEADLOCK_TH);	/** <--- SALIDA OBLIGATORIA*/

	}
return;
}
void MTXunLock(struct MutexObj* self, Collection* ThCol) {
	ThCol->Mensaje->remove(ThCol, self, NULL);
	switch ( pthread_mutex_unlock (&(self->unMutex))) {
		case EPERM:
			throw (EMTXLOCKD4OTHR,
			StringClass.newCon("Error en bloqueo: el hilo %d quizo desbloquear %s sin haberlo bloqueado",
			pthread_self(),
			self->getName(self)));
			break;
		case EINVAL:
			throw (EBADPARAM_TH, 
			StringClass.newCon("Error en bloqueo del mtx %s por el hilo %d",
			self->getName(self),
			pthread_self()));
			exit(EBADPARAM_TH);	/** <--- SALIDA OBLIGATORIA*/
	}

return;
}
void MTXdestruir (struct MutexObj* self,Collection* ThCol) {

	switch ( pthread_mutex_destroy  (&(self->unMutex))) {
		case EBUSY:
			throw (EDESTRBUSYMTX,
			StringClass.newCon("Error en mtx: el hilo %d intentó borrar al mtx %s, el cual esta bloqueado",
			pthread_self(),
			self->getName(self)));
			return;
	}

	ThCol->Mensaje->remove (ThCol, self, NULL);
return;
}


void MTXsetName (struct MutexObj* self, String* sName) {
	if (self->mutexName != NULL) {
		self->mutexName->destruir (self->mutexName);
	}
self->mutexName = sName->strclone (sName);
}

char* MTXgetName (struct MutexObj* self) {
	return self->mutexName->strval (self->mutexName);
}





/**PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP*/
MutexObj* MTXnew (String* sName) {
MutexObj* nuevo;
pthread_mutex_t errchkmutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;

/*PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;*/

	nuevo = CALLOC (1, sizeof(MutexObj));
	
	nuevo->unMutex = errchkmutex;

	nuevo->mutexName = sName->strclone (sName);

	nuevo->lock = MutexClass.lock ;
	nuevo->Lock = MutexClass.Lock ;
	nuevo->tryLock = MutexClass.tryLock ;
	nuevo->unLock = MutexClass.unLock ;
	nuevo->destruir = MutexClass.destruir ;
	nuevo->setName = MutexClass.setName ;
	nuevo->getName = MutexClass.getName ;

return nuevo;
}

MutexObj* MTXnewCon (char* sName) {
MutexObj* nuevo;
String* nombre;
	nombre = StringClass.new (sName);
	nuevo = MutexClass.new (nombre);
	nombre->destruir (nombre);
return nuevo;
}


void (*_______DestruirMtx) (MutexObj*, Collection*);
void  MutexClass_GoLive (void) {

	MutexClass.lock = MTXlock;
	MutexClass.Lock = MTXLock_r;
	MutexClass.tryLock = MTXtryLock;
	MutexClass.unLock = MTXunLock;
	_______DestruirMtx = MTXdestruir;
	MutexClass.destruir =_______DestruirMtx;
	MutexClass.setName = MTXsetName;
	MutexClass.getName = MTXgetName;
	MutexClass.new = MTXnew;
	MutexClass.newCon = MTXnewCon;

}

/*#define MUTEXEXAMPLE*/
#ifdef MUTEXEXAMPLE
#include "ult.h"

void handlerEDESTRBUSYMTX23 (ErrObj* pSelf) {
String* self = (String*) pSelf->extra;
	printf ("%s\n",self->strval(self));
/*exit (pSelf->getId(pSelf));*/
}

MutexObj* miMtx;
int ik = 0;
void* hilito (void* th) {
ULThread self = (ULThread) th;
Collection* col;
int i;
col = CollectionClass_New ();

	for (i = 0; i < 200 ; i++){
		miMtx->lock (miMtx,col);
		printf ("\t\tsoy Hilito [%d]\t %d --- ik: %d\n",pthread_self(),i, ik++);
		miMtx->unLock (miMtx,col);

	}
miMtx->unLock (miMtx,col);
pthread_detach (pthread_self());
return NULL;
}


int main (void) {
ULThread unThread;
int i;
Collection* col;
sigset_t Set;

	sigemptyset (&Set);
	sigaddset (&Set, SIGINT);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;


initErrMan(SIGINT);
colClassGoLive();
MutexClass_GoLive();
StringClass_GoLive();

miMtx = MutexClass.newCon ("MTX");

catch(23,handlerEDESTRBUSYMTX23);
catch(EDESTRBUSYMTX,handlerEDESTRBUSYMTX23);
unThread = ULThreadClass_new();
unThread->setFuncion (unThread, hilito);
unThread->setWorkSpace (unThread, unThread);
unThread->iniciarHilo (unThread);



col = CollectionClass_New ();
col->Mensaje->add (col, miMtx);

	for (i = 0; i < 300 ; i++){
		miMtx->lock (miMtx,col);
		printf ("soy main[%d] \t%d --- ik: %d\n", (unsigned int)pthread_self(),i,ik--);

		miMtx->unLock (miMtx,col);
	}
		
miMtx->unLock (miMtx,col);
sleep (1);
miMtx->destruir (miMtx,col);
return 0;
}



#endif








