#include "Server.h"


/** Info Class *************************************************************/



int setIp (___connInfo* objNfo, char*sIP) {
int iIP;
	if (sIP == NULL) {
		throw (EBADPARAM_SERV, objNfo);
		return -1;
	}
	iIP = inet_addr (sIP);
	if (iIP != INADDR_NONE) {
		objNfo->iIP = iIP;
		return 0;
	}
return -1;
}

int setPuerto (___connInfo*objNfo,unsigned short shPuerto) {
	if (shPuerto == 0) {
		throw (EBADPARAM_SERV, objNfo);
		return -1; 
	}
	objNfo->shPuerto = shPuerto;
return 0;
}

char* ITOC_IP (unsigned int* iIP){
  char* sIP = (char*) iIP;
  char* ssIP = (char*) MALLOC (sizeof (char) * 20) ;


  sprintf(ssIP,"%d.%d.%d.%d",(unsigned char) sIP[0],(unsigned char) sIP[1],
                            (unsigned char)sIP[2],(unsigned char) sIP[3]);
  return ssIP;
}


char* getIp (___connInfo* objNfo) {
unsigned int ip = objNfo->iIP;
	return ITOC_IP (&ip);
}

unsigned int getIntIp (___connInfo* objNfo) {
	return objNfo->iIP;
}
unsigned short getPuerto (___connInfo* objNfo) {
	return objNfo->shPuerto;
}

unsigned int getID (___connInfo*objNfo) {
	return objNfo->myID;
}


___connInfo* infoClassNew (___connInfoClass* InfoClass){
___connInfo* unInfo;
	unInfo = CALLOC (1, sizeof (___connInfo));
	unInfo->setIp = InfoClass->setIp ;
	unInfo->setPuerto = InfoClass->setPuerto ;
	unInfo->getIp = InfoClass->getIp ;
	unInfo->getIntIp = InfoClass->getIntIp ;
	unInfo->getPuerto = InfoClass->getPuerto ;
	unInfo->getID = InfoClass->getID ;

return unInfo;
}


void infoClassGoLive (void) {
	___InfoClass.setIp = setIp;
	___InfoClass.setPuerto = setPuerto;
	___InfoClass.getIp = getIp;
	___InfoClass.getIntIp = getIntIp;
	___InfoClass.getPuerto = getPuerto;
	___InfoClass.getID = getID;
	___InfoClass.new = infoClassNew;
}

/** *************************************************************/
/** *************************************************************/
/** Listener Class *************************************************************/


___listenerClass ListenerClass;

int iniciarListener (___listener* objListener, unsigned int iIP, unsigned short shPuerto) {


	objListener->fdListener->listen (objListener->fdListener, iIP, shPuerto, DEFAULT_BCKLOG);
	
return objListener->fdListener->getFd (objListener->fdListener);
}

int cerrarListener(___listener* objListener){
	objListener->iOldListener = objListener->fdListener->getFd (objListener->fdListener);
	objListener->fdListener->close (objListener->fdListener);

return objListener->iOldListener;
}

int getListener(___listener* objListener) {
return objListener->fdListener->getFd (objListener->fdListener);
}
int getOldListener(___listener* objListener) {
return objListener->iOldListener;
}

bool revisarListener (___listener* objListener, fd_set Set){
	if (objListener->getListener (objListener) == -1) return FALSE;
	return FD_ISSET (objListener->getListener (objListener), &Set);
}

FdObj* aceptarConexion (___listener* objListener) {
return  objListener->fdListener->accept (objListener->fdListener);
}

___listener* ListenerClassNew(struct ___listenerClass* listenerClass) {
___listener* unListener;
	unListener = CALLOC (1, sizeof (___listener));
	unListener->iniciarListener = listenerClass->iniciarListener ;
	unListener->cerrarListener = listenerClass->cerrarListener ;
	unListener->getListener = listenerClass->getListener ;
	unListener->getOldListener= listenerClass->getOldListener ;
	unListener->revisarListener = listenerClass->revisarListener ;
	unListener->aceptarConexion = listenerClass->aceptarConexion ;
	unListener->fdListener = FdObjClass.newCon(-1);


return unListener;
}

void ListenerClassGoLive (void) {
	ListenerClass.iniciarListener = iniciarListener ;
	ListenerClass.cerrarListener = cerrarListener ;
	ListenerClass.getListener = getListener ;
	ListenerClass.getOldListener= getOldListener ;
	ListenerClass.revisarListener = revisarListener ;
	ListenerClass.aceptarConexion = aceptarConexion ;
	ListenerClass.new = ListenerClassNew ;
}




/** *************************************************************/
/** *************************************************************/
/** Select Class *************************************************************/

struct ___selectClass SelectClass;

void inicializar(___select* objSelect) {
	FD_ZERO (&(objSelect->setClientes));
	FD_ZERO (&(objSelect->setFijo));
	objSelect->iMaxFD = 0;
	objSelect->iTimeOut = DEFAULT_TOUT;
	objSelect->tvTimeOut.tv_sec = 0;
	objSelect->tvTimeOut.tv_usec = 0;
return;
}

int setTimeOut (___select* objSelect, int unTiempo){
	objSelect->iTimeOut = unTiempo;
	return unTiempo;
}

int getTimeOut (___select* objSelect){
	return objSelect->iTimeOut;
}

fd_set getConjActivos (___select* objSelect) {
	return objSelect->setClientes;
	
}
	
int SetNuevoFd (___select* objSelect,int unFd) {
	if (unFd < 0) {
		throw (EBADPARAM_SERV, objSelect);
		return -1;
	}
	FD_SET (unFd, &(objSelect->setFijo));
	objSelect->iMaxFD = (objSelect->iMaxFD > unFd)? objSelect->iMaxFD:unFd;
return 0;
}
int CerrarFd (___select* objSelect,int unFd){
	if (unFd < 0){
		throw (EBADPARAM_SERV, StringClass.newCon ("Se intento cerrar un fd invalido (fd: %d)\n",unFd));
		return -1;
	}
	FD_CLR (unFd, &(objSelect->setFijo));

return 0;
}



int SELgetTimetoTimeOut (struct ___select* self) {

		return self->tvTimeOut.tv_sec + self->tvTimeOut.tv_usec / 1000000; 	
}

int esperarPeticiones(___select* objSelect) {
int iSelectRet;
	objSelect->setClientes = objSelect->setFijo;
	if (objSelect->tvTimeOut.tv_sec + objSelect->tvTimeOut.tv_usec == 0) {
		objSelect->tvTimeOut.tv_sec = objSelect->iTimeOut;
	}
	iSelectRet = select(objSelect->iMaxFD + 1,&(objSelect->setClientes) , NULL, NULL,&(objSelect->tvTimeOut));
	if (iSelectRet == -1)  {
		throw (ESELECT_SERV, objSelect);
	}
return iSelectRet;
}
int esperarPeticionesIndeterminado(___select* objSelect) {
int iSelectRet;
	objSelect->setClientes = objSelect->setFijo;
	iSelectRet = select(objSelect->iMaxFD + 1,&(objSelect->setClientes) , NULL, NULL,NULL);

	if (iSelectRet == -1)  {
		throw (ESELECT_SERV, objSelect);
	}
return iSelectRet;
}

___select* SelectClassNew(struct ___selectClass* __SelectClass) {
___select* objSelect;
	objSelect = CALLOC (1, sizeof (___select));
	objSelect->inicializar = __SelectClass->inicializar;
	objSelect->setTimeOut = __SelectClass->setTimeOut;
	objSelect->getTimeOut = __SelectClass->getTimeOut;
	objSelect->getConjActivos = __SelectClass->getConjActivos;
	objSelect->SetNuevoFd = __SelectClass->SetNuevoFd;
	objSelect->esperarPeticiones = __SelectClass->esperarPeticiones;
	objSelect->esperarPeticionesIndeterminado = __SelectClass->esperarPeticionesIndeterminado;
	objSelect->CerrarFd = __SelectClass->CerrarFd;
	objSelect->getTimetoTimeOut = __SelectClass->getTimetoTimeOut;

return objSelect;
}


void SelectClassGoLive  (void) {
	SelectClass.inicializar = inicializar;
	SelectClass.setTimeOut = setTimeOut;
	SelectClass.getTimeOut = getTimeOut;
	SelectClass.getConjActivos = getConjActivos;
	SelectClass.SetNuevoFd = SetNuevoFd;
	SelectClass.esperarPeticiones = esperarPeticiones;
	SelectClass.esperarPeticionesIndeterminado = esperarPeticionesIndeterminado;
	SelectClass.new = SelectClassNew;
	SelectClass.getTimetoTimeOut = SELgetTimetoTimeOut;
	SelectClass.CerrarFd = CerrarFd;
return;
}



/** CLIENTES!! */

int CLIconectarCon (struct ___client* self,char* sIP,unsigned short shPuerto) {
	return self->conectarCon_i (self, inet_addr(sIP), shPuerto);
}
int CLIconectarCon_i (struct ___client* self,unsigned int iIP,unsigned short shPuerto){
	return self->fdCliente->connect (self->fdCliente, iIP, shPuerto);
}



bool CLIigualA(struct ___client* self,int unFd) {
	return (self->fdCliente->getFd (self->fdCliente) == unFd);
}

int CLIgetDescriptor (struct ___client*self) {
	return (self->fdCliente->getFd (self->fdCliente));
}

int CLIsetDescriptor (struct ___client*self, FdObj* unFd) {
	self->fdCliente = unFd;
return 0;
}
int CLIsetEstado(struct ___client* self,fd_set unSet){	/* estado a Leer/leido */
	if (self->getDescriptor(self) < 0 ){
		throw (EBADPARAM_SERV, self);
		return -1;
	}
	if (self->iEstado == CERRAR) return CERRAR;

	if (FD_ISSET (self->getDescriptor(self), &unSet)){
		self->iEstado = LEER;
		return LEER;
	}
self->iEstado = LEIDO;
return LEIDO;
}

int CLIcerrar (struct ___client*self){		/* estado a Cerrar */
	
	self->iEstado = CERRAR;
return 0;
}

/** reescribir segùn protocolo :) */


int CLIleer(struct ___client* self,void* unMensaje, int iSize) {
int iRecibido;

	/*iSize = (iSize > 4092)? 4092:iSize;*/ 
	/** por restricciones del sistema operativo, no me va a dar mas de un bloque por vez*/
	
	/*unMensaje = CALLOC (iSize + 4 ,sizeof(char));*/
	
/** unmensaje+iTotal = ptr desde donde esta libre  iSize -iTotal = a recibir*/
		iRecibido = self->fdCliente->leer(self->fdCliente,unMensaje,iSize);

return iRecibido;
}

int CLIescribir (struct ___client* self, void* buffer, int iSize) {
int  iEscrito = 0;
char* buff;
	buff = (char*) buffer;
	/**	envio seguro de datos. me aseguro de mandar hasta el ultimo byte en cada envio. */

		iEscrito = self->fdCliente->escribir (self->fdCliente, buff, iSize);

		if (iEscrito == -1){
			return -1;
		}
		
return iEscrito;
}

int CLIgetEstado(struct ___client*self) {
	return self->iEstado;
}
int CLIaplicarProtocolo (struct ___client* self, void* srv) {
	printf ("El protocolo del cliente no ha sido generado!!!!!\n");
return -1;
}
void CLIsetInfoExtra (struct ___client* self, void* extra) {
	self->infoExtra = extra;

}
void CLIsetAplicaProtocolo (struct ___client* self,int (* proto) (struct ___client*, void*)) {
	self->aplicarProtocolo = (int(*)(Cliente,void*))proto;
}

void (*_________Clidestruir) (struct ___client*);

void CLIdestruir (struct ___client* self){
	self->fdCliente->close(self->fdCliente);
	FREE (self);
	return;
}
___client* ClientClassNew(void) {
___client* nuevoCliente;
	nuevoCliente = CALLOC (1,sizeof(___client));

	nuevoCliente->conectarCon = ClientClass.conectarCon ;
	nuevoCliente->conectarCon_i = ClientClass.conectarCon_i ;
	nuevoCliente->igualA = ClientClass.igualA ;
	nuevoCliente->getDescriptor = ClientClass.getDescriptor ;
	nuevoCliente->setDescriptor = ClientClass.setDescriptor;
    nuevoCliente->tipoCliente = NETCLIENT;
	nuevoCliente->setEstado = ClientClass.setEstado ;
	nuevoCliente->cerrar = ClientClass.cerrar ;
	nuevoCliente->leer = ClientClass.leer ;
	nuevoCliente->escribir = ClientClass.escribir ;
	nuevoCliente->getEstado = ClientClass.getEstado ;
	nuevoCliente->fdCliente= FdObjClass.new();
	nuevoCliente->setAplicaProtocolo = ClientClass.setAplicaProtocolo;
	nuevoCliente->setInfoExtra = ClientClass.setInfoExtra;
	nuevoCliente->aplicarProtocolo = ClientClass.aplicarProtocolo;
	nuevoCliente->destruir = ClientClass.destruir;
	
return nuevoCliente;
}
void ClientClassGoLive (void) {
	ClientClass.conectarCon = CLIconectarCon;
	ClientClass.conectarCon_i = CLIconectarCon_i ;
	ClientClass.igualA = CLIigualA ;
	ClientClass.getDescriptor = CLIgetDescriptor ;
	ClientClass.setDescriptor = CLIsetDescriptor ;
	ClientClass.setEstado = CLIsetEstado ;
	ClientClass.cerrar = CLIcerrar ;
	ClientClass.leer = CLIleer ;
	ClientClass.getEstado = CLIgetEstado ;
	ClientClass.new = ClientClassNew;
	ClientClass.setAplicaProtocolo = (void (*)(Cliente,int(*)(Cliente,void*)))CLIsetAplicaProtocolo;
	/** 
	 * se resuelve por casteo debido a que cliente se debe definir antes que servidor, por lo que cliente no podria
	 * recibir o tener un servidor asociado.
	 **/
	ClientClass.setInfoExtra = CLIsetInfoExtra;
	ClientClass.aplicarProtocolo = (int (*)(Cliente,void*))CLIaplicarProtocolo;
	ClientClass.escribir = CLIescribir;
	ClientClass.destruir = CLIdestruir;
}

/** *********************************************************/

/** *************************************************************/
/** *************************************************************/
/** Server Class *************************************************************/




int ssetIp (struct ____Server* objServer,char* sIP){
	return objServer->objInfo->setIp (objServer->objInfo, sIP);
}
int ssetPuerto (struct ____Server* objServer,unsigned short shPuerto){
	return objServer->objInfo->setPuerto (objServer->objInfo, shPuerto);
}
char* sgetIp (struct ____Server* objServer) {
	return objServer->objInfo->getIp (objServer->objInfo);
}
unsigned int sgetIntIp (struct ____Server* objServer){
	return objServer->objInfo->getIntIp  (objServer->objInfo);
}
unsigned short sgetPuerto (struct ____Server* objServer){
	return objServer->objInfo->getPuerto   (objServer->objInfo);
}
unsigned int sgetID (struct ____Server* objServer){
	return objServer->objInfo->getID   (objServer->objInfo);
}

/** listener path*/

int iniciarServidor (struct ____Server* objServer) {

	return objServer->SetNuevoCliente (objServer,
	(objServer->objListener->iniciarListener (objServer->objListener, objServer->getIntIp(objServer),objServer->getPuerto(objServer))));
}

int cerrarServidor(struct ____Server* objServer) {
	if (objServer->objSelect->CerrarFd (objServer->objSelect,
					objServer->objListener->getListener(objServer->objListener)) == -1) return -1;
	objServer->objListener->cerrarListener (objServer->objListener);
return 0;
}
/** select path*/
int ssetTimeOut (struct ____Server* objServer, int iTOut){
	return (objServer->objSelect->setTimeOut (objServer->objSelect, iTOut));
}

int sgetTimeOut (struct ____Server* objServer){
	return (objServer->objSelect->getTimeOut (objServer->objSelect));
}

fd_set sgetConjActivos (struct ____Server* objServer){
	return (objServer->objSelect->getConjActivos (objServer->objSelect));
}

int sSetNuevoCliente (struct ____Server* objServer,int unFd){
	return objServer->objSelect->SetNuevoFd (objServer->objSelect, unFd);
}




bool _____getCierres (___client* unCliente, fd_set* unSet) {

	unCliente->setEstado(unCliente,*unSet);
	return (unCliente->getEstado(unCliente) == CERRAR);
}

Collection* getCerrados(struct ____Server* objServer){
fd_set unSet;

	return (objServer->Contactos->Mensaje->select (objServer->Contactos,
				(bool(*)(void*,void*))_____getCierres,&unSet));
						
}

/** 
 * 	saca a todos los nodos a cerrar del conj Select, los elimina de la coleccion principal
 *  y libera los clientes. 
 * */
bool _____DesSetea (void* col, void*  cli, void* voidsrv) {
Server srv = (Server) voidsrv;
Cliente cl = (Cliente) cli;

	srv->objSelect->CerrarFd (srv->objSelect, cl->getDescriptor(cl));
	srv->Contactos->Mensaje->remove (srv->Contactos, cli, NULL);
	cl->destruir (cl);
return TRUE;

}


bool sesperarPeticiones(struct ____Server* objServer) {
FdObj* unFd;
fd_set unSet;
Cliente unCliente;
Collection* col;
	col = getCerrados (objServer);
	col->Mensaje->Do(col, _____DesSetea, objServer);
	col->Mensaje->destruir(col);
	
	objServer->objSelect->esperarPeticiones (objServer->objSelect);
	unSet = objServer->objSelect->getConjActivos (objServer->objSelect);
	if (objServer->objListener->revisarListener (objServer->objListener,unSet)) {
		unFd = objServer->objListener->aceptarConexion (objServer->objListener);
		objServer->objSelect->SetNuevoFd (objServer->objSelect,unFd->getFd (unFd));
		unCliente = ClientClass.new ();
		unCliente->setDescriptor (unCliente, unFd);
		objServer->msjAccept (objServer, unCliente);
		objServer->Contactos->Mensaje->add (objServer->Contactos,unCliente);
	}
	col = objServer->getActivos(objServer);
	col->Mensaje->destruir(col);
	
return 0;
}

bool sesperarPeticionesIndeterminado(struct ____Server* objServer){
FdObj* unFd;
fd_set unSet;
Cliente unCliente;
Collection* col;
	col = getCerrados (objServer);
	col->Mensaje->Do(col, _____DesSetea, objServer);
	col->Mensaje->destruir(col);
	objServer->objSelect->esperarPeticionesIndeterminado (objServer->objSelect);
	unSet = objServer->objSelect->getConjActivos (objServer->objSelect);
	if (objServer->objListener->revisarListener (objServer->objListener,unSet)) {
		unFd = objServer->objListener->aceptarConexion (objServer->objListener);
		objServer->objSelect->SetNuevoFd (objServer->objSelect,unFd->getFd (unFd));
		unCliente = ClientClass.new ();
		unCliente->setDescriptor (unCliente, unFd);
		objServer->msjAccept (objServer, unCliente);
		objServer->Contactos->Mensaje->add (objServer->Contactos,unCliente);
	}
	col = objServer->getActivos(objServer);
	col->Mensaje->destruir(col);
	return (0);
}
/** collection path*/

bool _____getActivo (___client* unCliente, fd_set* unSet) {

	unCliente->setEstado(unCliente,*unSet);
	return (unCliente->getEstado(unCliente) == LEER);
}


Collection* getActivos(struct ____Server* objServer){
fd_set unSet;

	unSet = objServer->objSelect->getConjActivos (objServer->objSelect);

	return (objServer->Contactos->Mensaje->select (objServer->Contactos,
				(bool(*)(void*,void*))_____getActivo,&unSet));
						
}

Collection* Seleccionar(struct ____Server* objServer,bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave){
	return (objServer->Contactos->Mensaje->select (
					objServer->Contactos,
					Condicion,unaClave
					)
		);
}


int Detectar (struct ____Server* objServer,bool (*Comparador) (void*,void* , void*), void* unaClave){
	return objServer->Contactos->Mensaje->detect (
					objServer->Contactos,
					Comparador,unaClave
					);
}



/**server path */

___client* conectarCliente (struct ____Server* objServer,char* sIP, unsigned short shPuerto) {
	___client* nuevoCliente;
	
	nuevoCliente = ClientClass.new ();
	if (nuevoCliente->conectarCon (nuevoCliente, sIP, shPuerto) == -1) {
			FREE(nuevoCliente);		
			return NULL;
	}
	objServer->objSelect->SetNuevoFd (objServer->objSelect,
					  nuevoCliente->getDescriptor(nuevoCliente));
	objServer->addCliente (objServer, nuevoCliente);
return nuevoCliente;
}
___client* conectarCliente_iip (struct ____Server* objServer,unsigned int iIP, unsigned short shPuerto){
	___client* nuevoCliente;
	
	nuevoCliente = ClientClass.new ();
	if (nuevoCliente->conectarCon_i (nuevoCliente, iIP, shPuerto)== -1) {
			FREE(nuevoCliente);		
			return NULL;
	}
	objServer->objSelect->SetNuevoFd (objServer->objSelect,
					  nuevoCliente->getDescriptor(nuevoCliente));
	 objServer->addCliente (objServer, nuevoCliente);
return nuevoCliente;
}
void CerrarCliente (struct ____Server* self,___client* unCliente){
	self->objSelect->CerrarFd(self->objSelect,unCliente->getDescriptor(unCliente));
	
}

int SRVgetTimetoTimeOut (struct ____Server* self) {
	return (self->objSelect->getTimetoTimeOut(self->objSelect));
}

bool seleccionaClientePorFd (void* cli, void* fd) {
int ifd = (int) fd;
___client* cl =(___client*) cli;

	return (cl->getDescriptor(cl) == ifd)? TRUE:FALSE;
}

___client* SRVgetCliente (struct ____Server* self,int fd) {
___client* cl;
Collection* col;
		col = self->Seleccionar(self,seleccionaClientePorFd, (void*)fd);
		cl = col->Mensaje->getFirst (col);
		col->Mensaje->destruir(col);
	return cl;
}

void SRVaddCliente (struct ____Server* self,___client* cl) {
	if (cl != NULL)
	self->Contactos->Mensaje->add(self->Contactos,cl);
}




____Server* ServerClassNew(void) {
____Server* objServer;
	objServer = CALLOC (1, sizeof (____Server));
	objServer->setIp = ServerClass.setIp ;
	objServer->setPuerto = ServerClass.setPuerto ;
	objServer->getIp = ServerClass.getIp ;
	objServer->getIntIp = ServerClass.getIntIp ;
	objServer->getPuerto = ServerClass.getPuerto ;
	objServer->getID = ServerClass.getID ;
	objServer->iniciarServidor = ServerClass.iniciarServidor ;
	objServer->cerrarServidor = ServerClass.cerrarServidor ;
	objServer->setTimeOut = ServerClass.setTimeOut ;
	objServer->getTimeOut = ServerClass.getTimeOut ;
	objServer->getConjActivos = ServerClass.getConjActivos ;
	objServer->SetNuevoCliente = ServerClass.SetNuevoCliente ;
	objServer->esperarPeticiones = ServerClass.esperarPeticiones ;
	objServer->esperarPeticionesIndeterminado = ServerClass.esperarPeticionesIndeterminado ;
	objServer->getActivos = ServerClass.getActivos ;
	objServer->Seleccionar = ServerClass.Seleccionar ;
	objServer->Detectar = ServerClass.Detectar ;
	objServer->conectarCliente = ServerClass.conectarCliente ;
	objServer->conectarCliente_iip = ServerClass.conectarCliente_iip ;
	objServer->CerrarCliente = ServerClass.CerrarCliente ;
	objServer->getCliente = ServerClass.getCliente;
	objServer->getTimetoTimeOut = ServerClass.getTimetoTimeOut;
	objServer->addCliente = ServerClass.addCliente;
	objServer->estaActivo = ServerClass.estaActivo;
	objServer->msjAccept = ServerClass.msjAccept;
	objServer->setMsjAccept = ServerClass.setMsjAccept;
/** creo los objetos adjuntos :)*/
	objServer->objSelect = SelectClassNew (&SelectClass);
	objServer->objInfo =  infoClassNew (&___InfoClass);
	objServer->objListener = ListenerClassNew (&ListenerClass);
	objServer->Contactos = CollectionClass.new();

return objServer;
}

int SRVestaActivo (struct ____Server* self) {
	return ((self->objListener->getListener (self->objListener)) != -1)? TRUE:FALSE;
}

int DFmsjAccept(struct ____Server* self,Cliente cl) {
	return 0;
}

void SRVsetMsjAccept (struct ____Server* self, int (*fn)(struct ____Server*,Cliente)){
	self->msjAccept = fn;
}
	

void ServerClassGoLive (void) {
	SelectClassGoLive (); 
	ListenerClassGoLive (); 
	infoClassGoLive ();
	ClientClassGoLive();

	ServerClass.setIp = ssetIp ;
	ServerClass.setPuerto = ssetPuerto ;
	ServerClass.getIp = sgetIp ;
	ServerClass.getIntIp = sgetIntIp ;
	ServerClass.getPuerto = sgetPuerto ;
	ServerClass.getID = sgetID ;
	ServerClass.iniciarServidor = iniciarServidor ;
	ServerClass.cerrarServidor = cerrarServidor ;
	ServerClass.setTimeOut = ssetTimeOut ;
	ServerClass.getTimeOut = sgetTimeOut ;
	ServerClass.getConjActivos = sgetConjActivos ;
	ServerClass.SetNuevoCliente = sSetNuevoCliente ;
	ServerClass.esperarPeticiones = sesperarPeticiones ;
	ServerClass.esperarPeticionesIndeterminado = sesperarPeticionesIndeterminado ;
	ServerClass.getActivos = getActivos ;
	ServerClass.Seleccionar = Seleccionar ;
	ServerClass.Detectar = Detectar ;
	ServerClass.conectarCliente = conectarCliente ;
	ServerClass.conectarCliente_iip = conectarCliente_iip ;
	ServerClass.CerrarCliente = CerrarCliente ;
	ServerClass.getTimetoTimeOut = SRVgetTimetoTimeOut;
	ServerClass.getCliente = SRVgetCliente;
	ServerClass.addCliente = SRVaddCliente;
	ServerClass.estaActivo = SRVestaActivo;
	ServerClass.new = ServerClassNew;

	ServerClass.msjAccept = DFmsjAccept;
	ServerClass.setMsjAccept = SRVsetMsjAccept ;
	
return;
}













#ifdef SERVEREXAMPLE 


bool nodosPolitician (Collection* unaColeccion, Cliente unCliente, Server unServidor){
char* unMensaje;
	
	if (unCliente == NULL) return FALSE;
	unMensaje = unCliente->leer (unCliente);
	if (unMensaje != NULL) {#define SERVEREXAMPLE 
		printf ("%s",unMensaje);
		FREE (unMensaje);
	}
	else {
		unServidor->CerrarCliente (unServidor,unCliente);
		exit (0);
	}

return TRUE;
}



void handlerWrite(struct ErrObj* unObjFd) {
	printf ("Error al escribir en un fd!! \n");
}
void handlerConnect (struct ErrObj* unObjFd) {
	printf ("Error al conectar! verifique que la información de IP/PUERTO es correcta\n");
return;
}


int main (void) {
int i;
Server unServidor;
Collection* unaColeccion;
Cliente unCliente;
char unaCadena [33];
sigset_t Set;
ServerClassGoLive();
initErrMan(SIGHUP);



	sigemptyset (&Set);
	sigaddset (&Set, SIGHUP);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;


unServidor = ServerClass.new (&ServerClass);
unServidor->setIp (unServidor, "127.0.0.1");
unServidor->setPuerto (unServidor, 9032);
unServidor->setTimeOut(unServidor, 20);
printf ("[padre] La ip es %s\n", unServidor->getIp (unServidor));
printf ("[padre] El puerto es %d\n", unServidor->getPuerto (unServidor));
printf ("[padre] El timeout es %d\n",unServidor->getTimeOut (unServidor));
if (!fork ()) {

unServidor->iniciarServidor(unServidor);
printf ("[padre] iniciando servidor... \n");

	for (i = 0; i < 1001; i++) {

		unServidor->esperarPeticionesIndeterminado (unServidor);

		unaColeccion = unServidor->getActivos (unServidor);

		unaColeccion->Mensaje->Do (unaColeccion, (bool(*)(void*,void*,void*))nodosPolitician, unServidor);
		unaColeccion->Mensaje->destruir (unaColeccion);

	}

}
else {
sleep (1);
	unCliente = clientClass.new (&clientClass);
	catch (ECONNECT_FD,handlerConnect);
	unCliente->conectarCon (unCliente,"127.0.0.1",9032);
	
usleep (100);
	catch (ECONNECT_FD,NULL);
	for (i = 0; i < 1000; i++) {
		sprintf (unaCadena, "\n**[MENSAJE %d]**\n",i);
		catch (EESCRITURA_FD,handlerWrite);
		unCliente->fdCliente->escribir (unCliente->fdCliente, unaCadena,strlen ("\n**[MENSAJE]**\n") +8);
		


	}
sleep (2);
}
return 0;
}


#endif



