#include "automan.h"

/**
 * el manager es 
 * */
/*typedef struct Automan {
	Automata* Frontend;
	Collection * Clientes;
	ULThread* Manager;
	void (*cargarConf)(struct Automan*,char*);
	void (*nuevoCliente) (struct Automan*); 
	void (*revisarBuffers) (struct Automan*, fd_set); 
	void (*iniciar)(struct Automan*);
	void (*agregarFn)(struct Automan*, (void(*)(void*,int,String**)), void*, int);
	void (*destruir)(struct Automan*);
}Automan;  
*/
 /**
  * Dado un archivo y un Automan, carga la configuración de automata. 
  *  
  **/
  
void* AutoManager (void*);

void AMANcargarConf(struct Automan* self,char* path) {
	if (self->Frontend != NULL) {
			self->Frontend->destruir(self->Frontend);
	}
	
	self->Frontend = AutomataClass.newCon (path);
}

/**
 * 	Dado un cliente, lo asigna a la cadena de clientes. 
 *  
 **/

void AMANnuevoCliente (struct Automan* self,___client* unCliente) {
		self->Clientes->Mensaje->add (self->Clientes, unCliente);
	
} 

/**
 *	Funcion de soporte para colecciones, para recuperar los nodos a servir.  
 **/
bool _____getActivoAutoman (void* cliente, void* g) {
___client* unCliente = (___client*) cliente;


	return (unCliente->getEstado(unCliente) == LEER);
}

/**
 *  Devuelve la subcoleccion de clientes listos para ser servidos. 
 **/
Collection* AMANgetActivos (struct Automan* self) {
	return self->Clientes->Mensaje->select (self->Clientes,_____getActivoAutoman, NULL);
}

/**
 *  setea la funcion de hilo y el workplace (Automan). Luego inicia el hilo. 
 **/

void AMANiniciar(struct Automan* self ) {
	self->Manager->setFuncion (self->Manager, AutoManager);
	self->Manager->setWorkSpace (self->Manager, self);
	self->Manager->iniciarHilo (self->Manager);
}


/**
 *	Agrega funciones en el directorio de funciones [para mas info ver: automata.c/.h] 
 **/

void AMANagregarFn(struct Automan* self , String* (*fn)(void*,int,String**), void* fst, int iFnCod) {
	self->Frontend->agregar_fn (self->Frontend, fn, iFnCod, fst);
}

/**
 *	libera recursos.  
 **/
void AMANdestruir(struct Automan* self) {
	self->Frontend->destruir(self->Frontend);
	self->Clientes->Mensaje->destruir (self->Clientes);
	self->Manager->detenerHilo (self->Manager);
}
Automan* AMANnewCon (String* nom) {
Automan * at;
	at = AutomanClass.new ();
	at->nombre->destruir (at->nombre);
	at->nombre = StringClass.newCon ("%s>",nom->strval (nom));
	return at;
	
}

Automan* AMANnew (void) {
	Automan* nuevo;
		nuevo = CALLOC (1, sizeof (Automan));
		nuevo->Clientes = CollectionClass_New();
		nuevo->nombre = StringClass.newCon ("sinnombre");
		nuevo->dc = DataControlClass.new();
		nuevo->Manager = ULThreadClass.new ();
		nuevo->Manager->setFuncion (nuevo->Manager, AutoManager);
		nuevo->Manager->setWorkSpace (nuevo->Manager, nuevo);
		
		nuevo->agregarFn = AutomanClass.agregarFn;
		nuevo->cargarConf = AutomanClass.cargarConf;
		nuevo->destruir = AutomanClass.destruir;
		nuevo->getActivos = AutomanClass.getActivos;
		nuevo->iniciar = AutomanClass.iniciar;
		nuevo->nuevoCliente = AutomanClass.nuevoCliente;
	return nuevo;
}

void (*______destruirAutoman) (Automan*);
void AutomanClass_GoLive (void) {
		ultClassGoLive ();
		colClassGoLive ();
		StringClass_GoLive();
		MutexClass_GoLive ();
		ConditionClass_GoLive ();
		DataControlClass_GoLive ();
		
		AutomanClass.agregarFn = AMANagregarFn;
		AutomanClass.cargarConf = AMANcargarConf;
		______destruirAutoman = AMANdestruir;
		AutomanClass.destruir = ______destruirAutoman;
		AutomanClass.getActivos = AMANgetActivos;
		AutomanClass.iniciar = AMANiniciar;
		AutomanClass.nuevoCliente = AMANnuevoCliente;
		AutomanClass.new = AMANnew;
		AutomanClass.newCon = AMANnewCon;

}
bool aplica (void* col, void* ato, void* clave) {
atomoExe* atom;
String* str; 
Cliente cl = (Cliente) clave;
	atom = (atomoExe*) ato;

	if (atom == NULL) {
		str = StringClass.new ("Comando invalido\n");
	}
	else {

		str = atom->Com->fnComando(atom->Com->fstParam, atom->argc, atom->argv);
	}
	
	if (str != NULL) {

		cl->fdCliente->escribir (cl->fdCliente, str->strval(str), str->strlen(str));
	}
	return TRUE; 	
}
void to_lower_c (char* line) {
int iLong, i;
	iLong = strlen (line);

	for (i = 0; i < iLong; i++) {
		line [i] = tolower (line[i]);
	}
}
void cargarArchivo (Automan* self, char* buffer, Cliente cl) {
Collection* col;
int i;

		for ( i = 0 ; i< strlen(buffer) ; i++ ){
			if (buffer [i] == '\n' ||buffer [i] == '\r') {
					buffer [i] = '\0';
			}
		}
		
		 to_lower_c (buffer);
		col = self->Frontend->parsearArchivo(self->Frontend, buffer);
		
		if (col != NULL) {
			col->Mensaje->Do(col, aplica,cl);
		}
	return;
}


bool doParse_and_Apply (void* col, void* obj, void* automan) {
	___client* unCliente = (___client*) obj;
	Automan* self = (Automan*) automan;
	char buff [300 + 2];
	int ifd, iread;
	atomoExe* at;
	char fst;
	String* feedback;
	
		memset (buff, '\0', 302);
/**	TODO: cambiar esta chanchada :-) */
		ifd = unCliente->getDescriptor (unCliente);
		
		iread = unCliente->fdCliente->leer (unCliente->fdCliente,buff, 300);
		if (iread == -1 || iread == 0) {
			unCliente->cerrar (unCliente);
			return TRUE;
		}
		
	/** registro el primer caracter */
		fst = buff [0];
	/** si es algun comando rapido de automan, lo ejecuto, sino sigue */
		switch (fst) {
			case SO_OUT:
					system (&buff[1]);
					write (unCliente->getDescriptor(unCliente),self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
					return TRUE;
			case CLEAR:
					system ("clear");
					write (unCliente->getDescriptor(unCliente),self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
					return TRUE;
			case EMPTY:
					write (unCliente->getDescriptor(unCliente),self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
					return TRUE;
			case SLASH:
					exit (0);
			case ARCH_LOAD:
					cargarArchivo (self,&buff[1],unCliente);
					write (unCliente->getDescriptor(unCliente),self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
					return TRUE;
		}
		/** parsea.. */
		to_lower_c (buff);
		at = self->Frontend->parsearLinea(self->Frontend, buff);
		
		/**  ejecuta (o informa error).. */
		if (at == NULL) {
			feedback = StringClass.new ("Comando invalido\n");
		}
		else {
			if (at->Com->fnComando == NULL){
				feedback = StringClass.new ("Comando no implementado!! \n");
			}
			else
				feedback = at->Com->fnComando(at->Com->fstParam, at->argc, at->argv);
		}
		if (feedback != NULL) {
			/**  devuelve feedback.. */
			unCliente->fdCliente->escribir (unCliente->fdCliente,feedback->strval(feedback), feedback->strlen(feedback));
			feedback->destruir(feedback);
		}
	write (unCliente->getDescriptor(unCliente),self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
	return TRUE;
	
}



void* AutoManager (void* wsp) {
Automan* self = (Automan*) wsp;
int iEstado = 0;
Collection* cl, *thMtxCol;

	thMtxCol = CollectionClass_New();
	pthread_detach (pthread_self());

	write (STDOUT_FILENO,self->nombre->strval(self->nombre), self->nombre->strlen(self->nombre));
	do {
		self->dc->desbloquear (self->dc,thMtxCol);	
			if (iEstado == HABILITADO) {
				cl = self->getActivos(self);
				cl->Mensaje->Do(cl,doParse_and_Apply,self);
				
			}
			
		
		self->dc->bloquear (self->dc,thMtxCol);
	}
	while ((iEstado = self->dc->esperarEstado(self->dc)) != TERMINAR);
	
	self->destruir(self);
return NULL;

}
















