#include "fd.h"


void FDclose (struct FdObj* self) {
	close (self->ifd);
	self->ifd = -1;
}

int FDleer(struct FdObj* self, void* dataIn, int size)  {
int iBytes;
	switch ((iBytes = read (self->ifd, dataIn, size))) {
		case 0: 
			throw (ECIERRE_FD, self);
			break;
		case -1:
			switch (errno) {
				case EAGAIN: 
				case EINTR:
					throw (ESINCRO_FD, self);
					break;
				default:
					throw (ELECTURA_FD, self);
			}
	}
return iBytes;
}

int FDescribir (struct FdObj* self, void* dataOut, int size) {

int iBytes;
	switch ((iBytes = write (self->ifd, dataOut, size))) {
		case 0:
			if (size > 0)
				throw (ECIERRE_FD, self);
			break; 
		case -1:
			switch (errno) {
				case EAGAIN: 
				case EINTR:
					throw (ESINCRO_FD, self);
					break;
				default:
					throw (EESCRITURA_FD, self);
			}
	}
return iBytes;
}



int  FDgetFd (struct FdObj* self) {
	return self->ifd;
}




void FDdestruir (struct FdObj* self) {
	FREE (self);
}

int FDconnect (struct FdObj* self, unsigned int iIP, unsigned short shPuerto){
struct sockaddr_in ISOCK;
        ISOCK.sin_family = AF_INET;
        ISOCK.sin_port = htons(shPuerto);
        ISOCK.sin_addr.s_addr = iIP;

        memset (&(ISOCK.sin_zero),'\0',8);

	/* Crea un socket  */
	if (self->ifd != -1) {
		throw (EBADPARAM_FD, self);
		return -1;
	}
        if ((self->ifd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw (ESOCKET_FD, self);
		return -1;
        }

	/* Adjunta una conexion al socket abierto */
        if ((connect (self->ifd,(struct sockaddr*)&ISOCK, sizeof (struct sockaddr))) == -1){	
		self->close (self);
		throw (ECONNECT_FD, self);
		return -1;
        }
return 0;
}


int FDlisten (struct FdObj* self, unsigned int iIP, unsigned short shPuerto, int iBackLog) {
struct sockaddr_in stSock;
int iFdFlags;
static const int SI = 1;

	/* Inicializa estructuras */
	stSock.sin_family = AF_INET;
        stSock.sin_port = htons(shPuerto);
	memset (&(stSock.sin_zero),'\0',8);
        stSock.sin_addr.s_addr = iIP;
	if (stSock.sin_addr.s_addr == (unsigned int) -1) {
		throw (EIPINVALIDA_FD, self);
		return -1;
	}
	if ((self->ifd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
		throw (ESOCKET_FD,self);
		return -1;
	}

	if (setsockopt (self->ifd, SOL_SOCKET, SO_REUSEADDR, &SI, sizeof(int)) == -1) {
		throw (ESOCKOPT_FD, self);
		return -1;
	}
	/* Reserva un puerto de comunicacion */
	if ((bind((self->ifd), (struct sockaddr*) (&stSock), sizeof (struct sockaddr_in))) == -1) {
		throw (EBIND_FD, self);
		return -1;
	}
	/* Pone el socket creado antes a escuchar conexiones en el puerto dado */
	if ((listen (self->ifd, iBackLog)) == -1){
		throw (ELISTEN_FD, self);
		return -1;
	}
	if  ((iFdFlags = fcntl (self->ifd, F_GETFL, 0) | O_NONBLOCK) == -1) {
		throw (EFCNTLGET_FD, self);
		return -1;
	}
	
	if (fcntl (self->ifd, F_SETFL, iFdFlags) == -1) {
		throw (EFCNTLSET_FD, self);
		return -1;
	}

return 0;
}

struct FdObj* FDaccept (struct FdObj* self) {
FdObj* nuevo;
int ifd;

	ifd = accept (self->ifd, NULL, 0);
	if (ifd == -1) {
		switch (errno) {
			case EAGAIN:
				throw (ESINCRO_FD, self);
				return NULL;
			case EBADF:
			case ENOTSOCK:
				throw (EBADPARAM_FD,self);
				return NULL;
		}
	}
	nuevo = FdObjClass.newCon (ifd);

return nuevo;

}

void FDopen (struct FdObj* self, char* sPath, int flags, mode_t modo) {
	if (self->ifd != -1 || sPath == NULL) {
		throw (EBADPARAM_FD, self);
		return;
	}
	self->ifd = open (sPath, flags,  modo);
	if (self->ifd == -1) {
		throw (EOPEN_FD, self);
	}
	return;
}




void (*________DestruirFDOBJ) (struct FdObj*);

FdObj* FdObjClass_new (void) {
FdObj* nuevo;
	nuevo = CALLOC (1, sizeof (FdObj));

	nuevo->close = FdObjClass.close;
	nuevo->leer = FdObjClass.leer ;
	nuevo->escribir = FdObjClass.escribir ;
	nuevo->getFd = FdObjClass.getFd ;
	nuevo->destruir = FdObjClass.destruir ;
	nuevo->connect = FdObjClass.connect;
	nuevo->listen = FdObjClass.listen;
	nuevo->accept = FdObjClass.accept;
	nuevo->open = FdObjClass.open;
	nuevo->ifd = -1;
return nuevo;
}

FdObj* FdObjClass_newCon (int ifd) {
FdObj* nuevo;
	nuevo = FdObjClass.new();
	nuevo->ifd = ifd;
return nuevo;
}

void FdObjClass_GoLive (void) {

	FdObjClass.close = FDclose;
	FdObjClass.leer = FDleer;
	FdObjClass.escribir = FDescribir;
	FdObjClass.getFd = FDgetFd;
	________DestruirFDOBJ = FDdestruir;
	FdObjClass.destruir = ________DestruirFDOBJ ;
	FdObjClass.connect = FDconnect;
	FdObjClass.listen = FDlisten;
	FdObjClass.accept = FDaccept;
	FdObjClass.open = FDopen;
	FdObjClass.new = FdObjClass_new;
	FdObjClass.newCon = FdObjClass_newCon;

}

















