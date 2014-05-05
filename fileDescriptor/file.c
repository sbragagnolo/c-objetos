#include "file.h"


void FILEopen (struct FileObj* self ,char* sPath, char* modo) {
	if (sPath == NULL || modo == NULL || self->Flujo != NULL) {
		throw (EBADPARAM_FD, self);
		return;
	}
	self->Flujo = fopen (sPath, modo);
	
	if (self->Flujo == NULL) {
		throw (EOPEN_FILE, self);
	}

return;
}

void FILEclose (struct FileObj* self) {
	if (self->Flujo == NULL) {
		throw (EBADPARAM_FD, self);
		return;
	}
	fclose (self->Flujo);
	self->Flujo = NULL;
return;
}

int FILEleer (struct FileObj* self, void* dataIn, int size) {
int iLeido = 0;
	if ((iLeido = fread(dataIn, size, 1, self->Flujo) <= 0)) {
		if (feof(self->Flujo)) {
			throw (EEOF_FILE, self);
		}
		if (ferror(self->Flujo)) {
			throw (EERR_FILE, self);
		}
	}
return iLeido;
}


int FILEescribir(struct FileObj* self, void* dataOut, int size) {
int iEscrito = 0;
	if (size <= 0) {
		throw (EBADPARAM_FD, self);
	}
	iEscrito = fwrite (dataOut, size, 1, self->Flujo);

	if (iEscrito <= 0) {
		if (ferror(self->Flujo)) {
			throw (EERR_FILE, self);
		}
	}
return iEscrito;
}

int FILEeof( struct FileObj* self) {
	return feof(self->Flujo);
}

int FILEerror ( struct FileObj* self) {
	return ferror(self->Flujo);
}

int FILEleerLinea(struct FileObj* self , void* dataIn, int size) {
	
	if (fgets(dataIn, size, self->Flujo) == NULL) {
		memset(dataIn,'\0',size);
		if (ferror(self->Flujo)) {
			throw (EERR_FILE, self);
			return 0;
		}
		if (feof(self->Flujo)) {
			throw (EEOF_FILE, self);
			return 0;
		}
	}
return 1;
}

void FILEdestruir (struct FileObj* self) {
	if (self->Flujo != NULL) {
		self->close (self);
	}
	free (self);
return;
}



FileObj* FILEnew (void) {
FileObj* nuevo;
	nuevo = CALLOC (1, sizeof (FileObj));

	nuevo->open = FileObjClass.open;
	nuevo->close = FileObjClass.close;
	nuevo->leer = FileObjClass.leer;
	nuevo->escribir = FileObjClass.escribir;
	nuevo->leerLinea = FileObjClass.leerLinea;
	nuevo->destruir = FileObjClass.destruir;
	nuevo->eof = FileObjClass.eof;
	nuevo->error = FileObjClass.error;
return nuevo;
}

FileObj* FILEnewCon (char* sPath, char* modo) {
FileObj* nuevo;
	nuevo = FileObjClass.new ();

	
	nuevo->open (nuevo,sPath, modo);
	if (nuevo->Flujo == NULL) {
		FREE(nuevo);
		return NULL;
	}
	return nuevo;
}

void (*________DestruirFileObj) (FileObj*);
void FileObjClass_GoLive (void) {

FileObjClass.open = FILEopen;
FileObjClass.close = FILEclose;
FileObjClass.leer = FILEleer;
FileObjClass.escribir = FILEescribir;
FileObjClass.leerLinea = FILEleerLinea;
________DestruirFileObj = FILEdestruir;
FileObjClass.destruir = ________DestruirFileObj ;
FileObjClass.new = FILEnew;
FileObjClass.newCon = FILEnewCon;
FileObjClass.eof = FILEeof;
FileObjClass.error = FILEerror;

}



#ifdef FILEEXAMPLE



void handlerEof (ErrObj* unObj) {
	printf("se ha llegado a fin de archivo!! \n");
exit (0);
}


int main (void) {
FileObj* unArchivo;
char Buffer [200 + 1];
int i = 0;
sigset_t Set;

	sigemptyset (&Set);
	sigaddset (&Set, SIGRTMIN);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;
FileObjClass_GoLive ();
initErrMan(SIGRTMIN);

unArchivo = FileObjClass.newCon ("/home/santiago/workspace/google", "r");

catch (EEOF_FILE, handlerEof);

while (1) {
	unArchivo->leerLinea (unArchivo, Buffer, 200);
	printf ("Linea:%d\t|\t%s",i++,Buffer);
}
catch (EEOF_FILE, NULL);
return 0;

}












#endif





