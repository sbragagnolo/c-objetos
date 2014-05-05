#include "logger.h"



void LOGescribir (struct LogObj* self,const char* formato,...) {
char unString [500 + 2];
va_list lista;
String* nuevo;

	va_start (lista, formato);
	vsnprintf(unString, 500, formato, lista);
	va_end (lista);
	nuevo = StringClass.new (unString);
	self->puts (self, nuevo);
	nuevo->destruir (nuevo);
return;
}
void LOGflushHeader (struct LogObj*self) {
time_t Hora;
char* sHora;
String* Header;

		time (&Hora);
		sHora = ctime (&Hora);
		sHora [strlen(sHora) - 1] = '\0';
	
		Header = StringClass.newCon (
	       "\n\n*********************** Archivo de Logueo ************************\
		\n  Creado(abierto) Por: %s (pid: %d)\
		\n  Hora: %s\
		\n******************************************************************\n\n",
		self->sOwner->strval (self->sOwner),
		getpid(),
		sHora );

		self->open (self);
		self->file->escribir (self->file, Header->strval(Header), Header->strlen(Header));
		self->close (self);
return;
}


void LOGputs (struct LogObj* self,String* unaEntrada){
time_t Hora;
char* sHora;
String* Log;

		time (&Hora);
		sHora = ctime (&Hora);
		sHora [strlen(sHora) - 1] = '\0';
		Log = StringClass.newCon ("[%s] [PID: %d] [OWNER: %s]: %s",
						sHora,
						getpid(),
						self->sOwner->strval (self->sOwner),
						unaEntrada->strval(unaEntrada));
		
		self->open (self);
		self->file->escribir (self->file, Log->strval(Log), Log->strlen(Log));
		self->close (self);
return;
}

void LOGopen (struct LogObj* self) {
	self->file = FileObjClass.newCon (self->sPathFile->strval(self->sPathFile), "a");
	
return;
}

void LOGclose (struct LogObj* self) {
	self->file->close(self->file);
}


void LOGsetPath (struct LogObj* self, String* sPath) {
	if (self->sPathFile != NULL) {
		self->sPathFile->destruir (self->sPathFile);
	} 

	self->sPathFile = sPath->strclone (sPath);

}
void LOGsetOwner(struct LogObj* self, String* sOwner){
	if (self->sOwner != NULL) {
		self->sOwner->destruir (self->sOwner);
	} 

	self->sOwner = sOwner->strclone (sOwner);
}
String* LOGgetPath (struct LogObj* self) {
	return self->sPathFile;
}
String* LOGgetOwner (struct LogObj* self) {
	return self->sOwner;
}

void LOGdestruir (struct LogObj* self) {
	if (self->sPathFile != NULL)
		self->sPathFile->destruir (self->sPathFile);
	if (self->sOwner != NULL)
		self->sOwner->destruir (self->sOwner);
	if (self->file != NULL)
		self->file->destruir (self->file);
	
}

LogObj* LOGnew (void) {
LogObj* nuevo;
	nuevo = CALLOC (1, sizeof (LogObj));

	nuevo->escribir = LogClass.escribir ;
	nuevo->puts = LogClass.puts ;
	nuevo->open = LogClass.open ;
	nuevo->setPath = LogClass.setPath ;
	nuevo->setOwner = LogClass.setOwner ;
	nuevo->getPath = LogClass.getPath ;
	nuevo->getOwner = LogClass.getOwner ;
	nuevo->close = LogClass.close ;
	nuevo->destruir = LogClass.destruir ;
	nuevo->flushHeader = LogClass.flushHeader;
return nuevo;
}


LogObj* LOGnewCon (const char* sPath,String* sOwner) {
String* unStr;
	unStr = StringClass.newCon(sPath);
	return LogClass.new_Con (unStr, sOwner);
}

LogObj* LOGnew_Con (String* sPath,String* sOwner) {
LogObj* nuevo;
	nuevo = LogClass.new ();
	nuevo->setPath (nuevo, sPath);
	nuevo->setOwner(nuevo, sOwner);
return nuevo;
}
void (*______DESTRUIRLogObj) (LogObj*);

void LogClass_GoLive (void) {
	StringClass_GoLive();
	FileObjClass_GoLive();
	LogClass.escribir = LOGescribir;
	LogClass.puts = LOGputs;
	LogClass.open = LOGopen;
	LogClass.setPath = LOGsetPath;
	LogClass.getPath = LOGgetPath;
	LogClass.setOwner = LOGsetOwner;
	LogClass.getOwner = LOGgetOwner;
	LogClass.close = LOGclose;
	LogClass.new = LOGnew;
	LogClass.newCon = LOGnewCon;
	LogClass.new_Con = LOGnew_Con;
	LogClass.flushHeader = LOGflushHeader;
	______DESTRUIRLogObj = LOGdestruir;
	LogClass.destruir = ______DESTRUIRLogObj;
}



#ifdef LOGGEREXAMPLE

int main (int argc, char* argv []) {
LogObj* Log;
String * Owner;
int i;

	if (argc != 2) {
		printf ("PATH!!\n");
		exit (0);
	}
	LogClass_GoLive ();
	Owner = StringClass.new ("PRUEBA");	
	Log = LogClass.newCon(argv[1],Owner);
	Log->flushHeader(Log);
	for (i = 0; i < 4000; i ++) {
		Log->escribir (Log, "%s\t|\t%d\t|%p\n", "HOLA MUNDO!!", i, Log);
	}
	Log->destruir(Log);
return 0;


}

#endif

















