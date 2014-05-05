#ifndef ___ERRMANN 
#define ___ERRMANN
#include "../Collections/DiccionarioHibrido.h"
#include "../Hilo/ult.h"
#include "../memLib/memManager.h"
#include "errobj.h"
#include <stdio.h>
#include <signal.h>
#include "../String/string.h"



typedef struct ErrMan {
	ULThread Manager;
	int SenialError;
	sigset_t SetBloqueo;
	ErrObj* ErrorOcurrido;
	DiccionarioHibrido* Directorio;
	int (*iniciar) (struct ErrMan*);
	int (*detener) (struct ErrMan*);
	void (*setSig) (struct ErrMan*, int signo);
	int (*senialError)(struct ErrMan*);
	void (*esperarError) (struct ErrMan*);
	ErrObj* (*errorOcurrido) (struct ErrMan*);
	ErrObj* (*altaError) (struct ErrMan* self, int iErr, void* pExtra,void(*handler)(ErrObj*), void(*porDefecto)(ErrObj*) );
	void (*catch) (struct ErrMan*, int iErr, void (*)(struct ErrObj*));
	void (*catch_force) (struct ErrMan*, int iErr, void (*)(struct ErrObj*));
	void (*throw) (struct ErrMan*, int iErr, void* Extra);
	void (*destruir) (struct ErrMan*);
}ErrMan;


ErrMan* errManager;

typedef struct ___ErrManClass {
	int (*iniciar) (struct ErrMan*);
	int (*detener) (struct ErrMan*);
	void (*setSig) (struct ErrMan*, int signo);
	int (*senialError)(struct ErrMan*);
	void (*esperarError) (struct ErrMan*);
	ErrObj* (*errorOcurrido) (struct ErrMan*);
	ErrObj* (*altaError) (struct ErrMan* self, int iErr, void* pExtra,void(*handler)(ErrObj*), void(*porDefecto)(ErrObj*) );
	void (*catch) (struct ErrMan*, int iErr, void (*)(struct ErrObj*));
	void (*catch_force) (struct ErrMan*, int iErr, void (*)(struct ErrObj*));
	void (*throw) (struct ErrMan*, int iErr, void* Extra);
	void (*destruir) (struct ErrMan*);
	ErrMan* (*new) (int signo);
}___ErrManClass;

___ErrManClass ErrManClass;

void ErrManClass_GoLive(void);

#define throw(ERR,EXTRA) errManager->throw (errManager,ERR,EXTRA); \
			usleep(1);

#define catch(ERR,HAND) errManager->catch (errManager,ERR,HAND);

#define catch_force(ERR,HAND) errManager->catch_force (errManager,ERR,HAND);

#define initErrMan(SIGNO) ErrManClass_GoLive(); 		\
			  errManager = ErrManClass.new(SIGNO);	\
			  errManager->iniciar(errManager);	


/** lista de errores a tirar */

typedef enum {
	ECONEXION 	= 1,	/** error al conectar ??????*/
	ESOCKET_FD	= 2,    /** error al crear socket*/ 
	ECIERRE_FD	= 3,    /** error al cerrar socket */
	ELECTURA_FD	= 4,    /** error de lectura de socket */
	ESINCRO_FD	= 5,    /** error de sincronización E/S*/
	EESCRITURA_FD	= 6,    /** error de escritura */
	EBADPARAM_FD	= 7,	/** error de parametros en fd*/
	EOPEN_FILE	= 8,	/** error de apertura de archivo*/
	ECONNECT_FD	= 9,	/** error al conectar  */
	EIPINVALIDA_FD	= 10,	/** error de IP invalida*/
	ESOCKOPT_FD	= 11,	/** error de opcion de Socket */
	EBIND_FD	= 12,	/** error de enlace de puerto*/
	ELISTEN_FD	= 13,	/** error al iniciar listener */
	EFCNTLGET_FD	= 14,	/** error al obtener configuración de socket */
	EFCNTLSET_FD	= 15,	/** error al setear configuración de socket */
	EBADPARAM_SERV	= 16,	/** error de parametros en servidor*/
	ESELECT_SERV	= 17,	/** error de syscall select*/
	EOPEN_FD	= 18,	/** error de apertura de archivo como fd*/
	EERR_FILE	= 19,	/** error de archivo*/
	EEOF_FILE	= 20,	/** señal de fin de archivo*/
	EBADPARAM_TH	= 21,	/** error de parametros en mutex/hilo*/
	EDEADLOCK_TH	= 22,	/** error de bloqueo: posible deadlock*/
	EMTXLOCKD4OTHR	= 23,	/** error al desbloquear mtx no lock por mi*/
	EDESTRBUSYMTX	= 24,	/** error destruir mtx: esta bloqueado */
	ECORRUPTHEADER	= 25,	/** error al recibir header */
	EBADPARAM_AUT	= 26,	/** error en parameteros (automata) 			FATAL*/
	EESTADOREP_AUT	= 27, 	/** error el estado esta repetido 				FATAL */
	EESTADOINVALIDO = 28,	/** error de estado invalido  					FATAL*/
	EINSDICT		= 29,	/** error al insertar en diccionario  			FATAL*/
	EESTINIINX		= 30,	/** error, el automata no tiene estado inicial 	FATAL*/
	ESTXAUT			= 31,	/** error de sintaxis */
	ECOMNOIMP		= 32    /** error de comando no implementado 			*/

} LISTA_ERRORES;
	








#endif
