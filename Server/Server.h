#ifndef __SERVIDOR
#define __SERVIDOR
#include "../errMan/errman.h"
#include "../Collections/Collection.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../memLib/memManager.h"
#include "../fileDescriptor/fd.h"






#define DEFAULT_BCKLOG 10
#define DEFAULT_TOUT 3600


static const int SI = 1;


/** Class Info ********************************************/
typedef struct ___connInfo {
	unsigned int iIP;
	unsigned short shPuerto;
	unsigned int myID;
	int (*setIp) (struct ___connInfo*,char*);
	int (*setPuerto) (struct ___connInfo*,unsigned short);
	char* (*getIp) (struct ___connInfo*);
	unsigned int (*getIntIp) (struct ___connInfo*);
	unsigned short (*getPuerto) (struct ___connInfo*);
	unsigned int (*getID) (struct ___connInfo*);
}___connInfo;

typedef struct ___connInfoClass {
	int (*setIp) (struct ___connInfo*,char*);
	int (*setPuerto) (struct ___connInfo*,unsigned short);
	char* (*getIp) (struct ___connInfo*);
	unsigned int (*getIntIp) (struct ___connInfo*);
	unsigned short (*getPuerto) (struct ___connInfo*);
	unsigned int (*getID) (struct ___connInfo*);
	___connInfo* (*new) (struct ___connInfoClass*);
}___connInfoClass;
___connInfoClass ___InfoClass;

/** Class Listener ********************************************/
typedef struct ___listener {
	FdObj* fdListener;
	int iOldListener;
	int (*iniciarListener) (struct ___listener*,unsigned int, unsigned short);
	int (*cerrarListener)(struct ___listener*);
	int (*getListener)(struct ___listener*);
	int (*getOldListener)(struct ___listener*);
	bool (*revisarListener) (struct ___listener*,fd_set);
	FdObj* (*aceptarConexion)(struct ___listener*);
}___listener;


typedef struct ___listenerClass {
	int (*iniciarListener) (struct ___listener*,unsigned int, unsigned short);
	int (*cerrarListener)(struct ___listener*);
	int (*getListener)(struct ___listener*);
	int (*getOldListener)(struct ___listener*);
	bool (*revisarListener) (struct ___listener*,fd_set);
	FdObj* (*aceptarConexion)(struct ___listener*);
	___listener* (*new)(struct ___listenerClass*);
}___listenerClass;
/**		Clase cliente */

typedef enum {
	LEIDO = 0,
	LEER = 1,
	CERRAR = 2,
	ELIMINAR = 3
}estados;

typedef enum {
	NETCLIENT = 1,	/* por conexion o por aceptacion */
	ESPECIAL = 2,   /* entrada estandar, salida estandar, telnet, etc */
	ARCHIVO = 3     /* archivo, audio, etc */
}tipo;
typedef struct ___client { /* nodo bàsico del servidor :) */
	FdObj* fdCliente;
	int iEstado;
	int tipoCliente;
	char* buffer;
	int iBuffSize;
	void* infoExtra;
	int (*conectarCon) (struct ___client*,char*,unsigned short);
	int (*conectarCon_i) (struct ___client*, unsigned int,unsigned short);
	bool (*igualA)(struct ___client*,int);
	int (*setDescriptor) (struct ___client*, FdObj*);
	int (*getDescriptor) (struct ___client*);
	int (*setEstado)(struct ___client*,fd_set);	/* estado a Leer/leido */
	int (*cerrar) (struct ___client*);		/* estado a Cerrar */
	int (*leer)(struct ___client*,void*, int);
	int (*escribir)(struct ___client*, void*, int);
	int (*getEstado)(struct ___client*);
	int (*aplicarProtocolo) (struct ___client*,void*); /** ESTE METODO DEBE SER REESCRITO SIEMPRE!!!!! */
	void (*setInfoExtra) (struct ___client*,void*);
	void (*setAplicaProtocolo) (struct ___client*,int (*) (struct ___client*,void*));
	void (*destruir) (struct ___client*);
}___client;

typedef struct ___clientClass {
	int (*conectarCon) (struct ___client*,char*,unsigned short);
	int (*conectarCon_i) (struct ___client*,unsigned int,unsigned short);
	bool (*igualA)(struct ___client*,int);
	int (*getDescriptor) (struct ___client*);
	int (*setDescriptor) (struct ___client*, FdObj*);
	int (*setEstado)(struct ___client*,fd_set);	/* estado a Leer/leido */
	int (*cerrar) (struct ___client*);		/* estado a Cerrar */
	int (*leer)(struct ___client*,void*, int);
	int (*escribir)(struct ___client*, void*, int);
	int (*getEstado)(struct ___client*);
	int (*aplicarProtocolo) (struct ___client*,void*); /** ESTE METODO DEBE SER REESCRITO SIEMPRE!!!!! */
	void (*setInfoExtra) (struct ___client*, void*);
	void (*setAplicaProtocolo) (struct ___client*,int (*) (struct ___client*,void*));
	void (*destruir) (struct ___client*);
	___client* (*new) (void);
}___clientClass;

typedef ___client* Cliente;

/** Class Select ********************************************/
typedef struct ___select {
	fd_set setClientes;
	fd_set setFijo;
	int iMaxFD;
	int iTimeOut;
	struct timeval tvTimeOut;
	void (*inicializar)(struct ___select*);
	int (*setTimeOut) (struct ___select*, int);
	int (*getTimeOut) (struct ___select*);
	int (*getTimetoTimeOut) (struct ___select*);
	fd_set (*getConjActivos) (struct ___select*);
	int (*SetNuevoFd) (struct ___select*,int);
	int (*CerrarFd) (struct ___select*,int);
	int (*esperarPeticiones)(struct ___select*);
	int (*esperarPeticionesIndeterminado)(struct ___select*);
}___select;
typedef struct ___selectClass {
	void (*inicializar)(struct ___select*);
	int (*setTimeOut) (struct ___select*, int);
	int (*getTimeOut) (struct ___select*);
	int (*getTimetoTimeOut) (struct ___select*);
	fd_set (*getConjActivos) (struct ___select*);
	int (*SetNuevoFd) (struct ___select*,int);
	int (*CerrarFd) (struct ___select*,int);
	int (*esperarPeticiones)(struct ___select*);
	int (*esperarPeticionesIndeterminado)(struct ___select*);
	___select* (*new)(struct ___selectClass*);
}___selectClass;



/** Class Servidor ********************************************/
typedef struct ____Server{
	___connInfo* objInfo;
        ___listener* objListener;
	___select* objSelect;
	Collection* Contactos;
	void* espacioUtil;
	int (*msjAccept)(struct ____Server*,Cliente);
	void (*setMsjAccept) (struct ____Server*, int (*)(struct ____Server*,Cliente));
/** conn info path*/
	int (*setIp) (struct ____Server*,char*);
	int (*setPuerto) (struct ____Server*,unsigned short);
	char* (*getIp) (struct ____Server*);
	unsigned int (*getIntIp) (struct ____Server*);
	unsigned short (*getPuerto) (struct ____Server*);
	unsigned int (*getID) (struct ____Server*);
/** listener path*/
	int (*iniciarServidor) (struct ____Server*);
	int (*cerrarServidor)(struct ____Server*);
	int (*estaActivo) (struct ____Server*);
/** select path*/
	int (*setTimeOut) (struct ____Server*, int);
	int (*getTimeOut) (struct ____Server*);
	int (*getTimetoTimeOut) (struct ____Server*);
	fd_set (*getConjActivos) (struct ____Server*);
	int (*SetNuevoCliente) (struct ____Server*,int);
	bool (*esperarPeticiones)(struct ____Server*);
	bool (*esperarPeticionesIndeterminado)(struct ____Server*);
/** collection path*/
	Collection* (*getActivos)(struct ____Server*); 
	Collection* (*Seleccionar)(struct ____Server*,bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
	int (*Detectar) (struct ____Server*,bool (*Comparador) (void*,void* unObjetoDeCol, void*unaClave), void* unaClave);
	void (*addCliente) (struct ____Server*,___client*);
/**server path */
	___client* (*conectarCliente) (struct ____Server*,char*,unsigned short);
	___client* (*conectarCliente_iip) (struct ____Server*,unsigned int,unsigned short);
	___client* (*getCliente) (struct ____Server*,int);
	void (*CerrarCliente) (struct ____Server*,___client*);

}____Server;

typedef struct ____ServerClass{
/** conn info path*/
	int (*msjAccept)(struct ____Server*,Cliente);
	void (*setMsjAccept) (struct ____Server*, int (*)(struct ____Server*,Cliente));
	int (*setIp) (struct ____Server*,char*);
	int (*setPuerto) (struct ____Server*,unsigned short);
	char* (*getIp) (struct ____Server*);
	unsigned int (*getIntIp) (struct ____Server*);
	unsigned short (*getPuerto) (struct ____Server*);
	unsigned int (*getID) (struct ____Server*);
	int (*iniciarServidor) (struct ____Server*);
	int (*cerrarServidor)(struct ____Server*);
	int (*estaActivo) (struct ____Server*);
	int (*setTimeOut) (struct ____Server*, int);
	int (*getTimetoTimeOut) (struct ____Server*);
	int (*getTimeOut) (struct ____Server*);
	fd_set (*getConjActivos) (struct ____Server*);
	int (*SetNuevoCliente) (struct ____Server*,int);
	bool (*esperarPeticiones)(struct ____Server*);
	bool (*esperarPeticionesIndeterminado)(struct ____Server*);
	Collection* (*getActivos)(struct ____Server*); 
	Collection* (*Seleccionar)(struct ____Server*,bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
	int (*Detectar) (struct ____Server*,bool (*Comparador) (void*, void* unObjetoDeCol, void*unaClave), void* unaClave);
	void (*addCliente) (struct ____Server*,___client*);
	___client* (*conectarCliente) (struct ____Server*,char*,unsigned short);
	___client* (*conectarCliente_iip) (struct ____Server*,unsigned int,unsigned short);
	___client* (*getCliente) (struct ____Server*,int);
	void (*CerrarCliente) (struct ____Server*,___client*);
	____Server* (*new)(void);
}____ServerClass;

typedef ____Server* Server;

____ServerClass ServerClass;
void ServerClassGoLive (void);

___clientClass ClientClass;
void ClientClassGoLive (void);


#endif
