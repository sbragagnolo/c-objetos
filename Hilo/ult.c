

#include "ult.h"




void setFuncion (ULThread objThread, void*(*fn)(void*)) {
	objThread->fnAsociada = fn;
}

void setWorkSpace (ULThread objThread, void* WorkSpace){
	objThread->workSpace = WorkSpace;
return;
}




void* getWorkSpace (ULThread objThread) {
	return objThread->workSpace;
}

int getTid (ULThread objThread) {
	return objThread->TID;
}

int iniciarHilo(ULThread objThread) {
	if (objThread->TID != -1) return -1;
	return pthread_create (&(objThread->TID),
				NULL,
				objThread->fnAsociada,
				objThread->workSpace);

}


int detenerHilo(ULThread objThread) {
if (objThread->TID != -1) return -1;
	return pthread_kill (objThread->TID, SIGKILL);

}

struct ___ult___* ultNew (void) {
ULThread objThread;
	
	objThread = CALLOC (1,ULTSIZE);
	
	objThread->TID = -1;
	objThread->setFuncion = ULThreadClass.setFuncion;
	objThread->setWorkSpace = ULThreadClass.setWorkSpace;
	objThread->getWorkSpace = ULThreadClass.getWorkSpace;
	objThread->getTid = ULThreadClass.getTid;
	objThread->iniciarHilo = ULThreadClass.iniciarHilo;
return objThread;
}


 
void ultClassGoLive (void) {
	ULThreadClass.new = ultNew;
	ULThreadClass.setFuncion = setFuncion;
	ULThreadClass.setWorkSpace = setWorkSpace;
	ULThreadClass.getWorkSpace = getWorkSpace;
	ULThreadClass.getTid = getTid;
	ULThreadClass.iniciarHilo = iniciarHilo;
}






#ifdef EJEMPLO
#include <stdio.h>

void* counter (void* p) {
ULThread objThread = (ULThread) p;
int i;

	for (i = 0; i < 20; i++) {
		printf ("[TID:%d] [i:%d] \n",objThread->getTid(objThread),i);
	}
pthread_detach (objThread->getTid(objThread));
return NULL;
}

int main (void){
ULThread unThread, otroThread;
	ultClassGoLive ();
	unThread = ULThreadClass_new();
	unThread->setFuncion (unThread, counter);
	unThread->setWorkSpace (unThread, unThread);
	unThread->iniciarHilo (unThread);

	sleep(2);
}


#endif


