#include "Mensajes.h"

int iSequencia = 0;
/** **************  SECCION HEADER  ********************/

/** ********************************************************/
/** Compara IDs de header. Dos header son iguales si lo son*/
/** sus ID's. Sin importar el resto de los atributos.      */
/** ********************************************************/

bool header_esIgualA (struct MsjHeader* self, struct MsjHeader* comparado) {

/** compara el ID completo: ip entera, puerto y secuencia local. */
	if (self->msjId.iIP == comparado->msjId.iIP &&			
	    self->msjId.shPuerto == comparado->msjId.shPuerto &&
	    self->msjId.iSeqLocal == comparado->msjId.iSeqLocal) return TRUE;

return FALSE;

}



/** ********************************************************/
/** Recibe la configuracion de un header a traves de un    */
/** paquete extraido de un fd. (archivo/coneccion).        */
/** ********************************************************/

int header_recibirDe (struct MsjHeader* self, FdObj* fd) {
int iRecibido;
	memset (self, '\0', sizeof (MsjHeader));
	iRecibido = fd->leer (fd,self,sizeof (MsjHeader));
	if (iRecibido != sizeof (MsjHeader)) {
		throw (ECORRUPTHEADER, self); 
		/** si el paquete recibido no es del tamaño de 
		    header es por que no llego completo. Al ser un paquete muy chico
		    es muy poco probable que llegue en dos paquetes, por lo que debe 
		    informacion erronea.
		 */
		return -1;
	}
return iRecibido;
}


/** ********************************************************/
/** Crea un header completamente nuevo a partir de un     e*/
/** paquete extraido de un fd. (archivo/coneccion).        */
/** ********************************************************/


MsjHeader* header_crearDesde (FdObj* fd) {
MsjHeader* nuevoHeader;
	nuevoHeader = CALLOC (1, sizeof (MsjHeader));

/** Aloca memoria para el nuevo TAD y usa la funcion 
    recibirDe para configurarlo. */

	if (header_recibirDe (nuevoHeader, fd) == -1) {
		FREE (nuevoHeader);
		return NULL;
	}

return nuevoHeader;
}

/** ********************************************************/
/** Crea un header en base a informacion obtenida por      */
/** parametros (necesario para la creacion de msj locales) */
/** ********************************************************/

MsjHeader* header_Crear (MsjHeader* nuevoHeader,unsigned int iIP, unsigned short shPuerto, unsigned char cTTL, unsigned char cHOPS, unsigned char cTipo, unsigned int iLongMsj) {
	nuevoHeader->msjId.iIP = iIP;
	nuevoHeader->msjId.shPuerto = shPuerto;
	nuevoHeader->msjId.iSeqLocal = iSequencia;

	nuevoHeader->cHOPS = cHOPS;
	nuevoHeader->cTTL = cTTL;
	nuevoHeader->cTipoMensaje = cTipo;
	nuevoHeader->iLongMsj = iLongMsj;
	
	iSequencia++;
return nuevoHeader;
}


/** ********************************************************/
/** Revisa el estado actual del campo hops. Si llego al    */
/** limite de vida retorna falso, sino verdadero.          */
/** ********************************************************/


bool header_puedoRoutear(struct MsjHeader* self) {
	return (self->cTTL > self->cHOPS)? TRUE:FALSE;
}


/** ********************************************************/
/** Seters del Header.                                     */
/** nota: de ser necesario implementar getters ().         */
/** ********************************************************/

void header_setTTL (struct MsjHeader* self, unsigned char cTtl) {
	self->cTTL = cTtl;
return;
}

void header_setHOPS (struct MsjHeader* self, unsigned char cHOPS) {
	self->cHOPS = cHOPS;
return;
}

void header_incHOPS (struct MsjHeader* self) {
	self->cHOPS++;
return;
}

void header_setTipoMsj (struct MsjHeader* self, unsigned char cTipo) {
	self->cTipoMensaje = cTipo;
return;
}

void header_setLong (struct MsjHeader* self, unsigned int iLong) {
	self->iLongMsj = iLong;
return;
}

/** getters */

unsigned char header_getTTL (struct MsjHeader* self) {
	return self->cTTL;
}

unsigned char header_getHOPS (struct MsjHeader* self) {
	return self->cHOPS;
}

unsigned char header_getTipoMsj (struct MsjHeader* self) {
	return self->cTipoMensaje;
}

unsigned int header_getLong (struct MsjHeader* self) {
	return self->iLongMsj;
}

unsigned int header_getSum (struct MsjHeader* self) {
	return self->iSum;
}


/** **************  SECCION MENSAJE  ********************/

bool MSJesIgualA(struct Mensaje* self, struct Mensaje* otro) {
	return header_esIgualA (&(self->header), &(otro->header));
}

int MSJrecibirDe (struct Mensaje* self, FdObj* fd) {
int iRecibido = 0;
int iTotal = 0;
int iArecibir;
int iRecibir;
char* unMensaje;

/** Recibo el encabezado del mensaje */
	if (header_recibirDe (&(self->header), fd) == -1) return -1;
	iArecibir = header_getLong (&(self->header));
/** en base al encabezado, obtengo la longitud del mensaje real */
	unMensaje = CALLOC (1, iArecibir + 1);
	iRecibir = iArecibir;
/** me aseguro de recibir la totalidad de la longitud del mensaje. */
	while (iTotal < iArecibir) {
		iRecibido = fd->leer (fd,unMensaje + iTotal,iRecibir);

/**	si tengo problemas de recepcion, tengo que capturar el error de la 
	capa inferior y aca libero recursos. (nota: asegurarse de que no es 
	necesario tirar un error (throw)) */
		if (iRecibido == -1) { 
			FREE (unMensaje);
			self->pMensaje = NULL;
			return -1;
		}
		iTotal += iRecibido;
		iRecibir -=iRecibido;
		iRecibido = 0;
	}
/** recibi todo el mensaje bien, lo incrusto en la estructura de mensaje, devuelvo
el total recibido. */
self->pMensaje = unMensaje;
return iTotal;
}


int MSJenviarA (struct Mensaje* self, FdObj* fd) {
char* unPaquete;
int iSize;
int iEnviado = 0;
int iAEnviar;
/** Creo un char* para empaquetar el mensaje (char* por el manejo byte x byte)*/
	iSize = sizeof (MsjHeader) + header_getLong (&(self->header));
	unPaquete = CALLOC (1,iSize);
	iAEnviar = iSize;
	/** inserto el mensaje en una sola hilera de bytes. 
	(al estar el mensaje en un void*, el verdadero msj esta en algun 
	lugar de memoria) para poder enviarlo en un solo llamado.  */

	memcpy (unPaquete, &(self->header), sizeof (MsjHeader));
	memcpy (&(unPaquete [sizeof(MsjHeader)]), 
		self->pMensaje, header_getLong (&(self->header)));

	while ((iEnviado += fd->escribir (fd, unPaquete+iEnviado, iAEnviar)) < iSize) {
		if (iEnviado == -1) {
	/**
		NOTA: fijarse si es necesario throwear un error 
	*/
			return iEnviado;
		}
		iAEnviar -= iEnviado;
	}

return iEnviado;
}


bool MSJpuedoRoutear (struct Mensaje* self) {
	return header_puedoRoutear(&(self->header));
}

void MSJsetTTL (struct Mensaje* self, unsigned char cTTL) {
	header_setTTL (&(self->header), cTTL);
}
void MSJsetHOPS (struct Mensaje* self, unsigned char cHOPS) {
	header_setHOPS (&(self->header), cHOPS);
}
void MSJincHOPS (struct Mensaje* self) {
	header_incHOPS (&(self->header));
}
void MSJsetTipoMsj (struct Mensaje* self, unsigned char cTipo) {
	header_setTipoMsj (&(self->header), cTipo);
}
/*
void MSJsetMsj (struct Mensaje* self, void* msj, unsigned int iLong) {
	self->pMensaje = msj;
	header_setLong (&(self->header), iLong);
	self->header.iSum = self->getSum (self);
}
*/
/** ****************************************************************************
	Suma el codigo de cada bloque del mensaje. Este valor se incluye en el
	header con el fin de corroborar que el mensaje que llegó es valido. 
********************************************************************************/
unsigned int MSJgetSum (struct Mensaje* self) {
int i, j;
char* unMsj;
unsigned int iSum = 0;

	j = header_getLong (&(self->header));
	unMsj = (char*) self->pMensaje;
	for (i = 0; i < j ; i++) {
		iSum += unMsj [i];
	}
return iSum;
}


void MSJdestruir (struct Mensaje*self) {
	FREE(self);
}








/*

struct Mensaje* MSJnew (void) {
Mensaje* nuevo;

	nuevo = CALLOC (1,sizeof(Mensaje));

	nuevo->header.msjId.iIP = MsjClass.iIP;
	nuevo->header.msjId.shPuerto = MsjClass.shPuerto;
	nuevo->header.msjId.iSeqLocal = MsjClass.iSequence;
	MsjClass.iSequence ++;

	nuevo->esIgualA = MsjClass.esIgualA ;
	nuevo->recibirDe = MsjClass.recibirDe ;
	nuevo->enviarA = MsjClass.enviarA ;
	nuevo->puedoRoutear = MsjClass.puedoRoutear ;
	nuevo->getSum = MsjClass.getSum ;
	nuevo->setTTL = MsjClass.setTTL ;
	nuevo->setHOPS = MsjClass.setHOPS ;
	nuevo->incHOPS = MsjClass.incHOPS ;
	nuevo->setTipoMsj = MsjClass.setTipoMsj ;
	nuevo->setMsj = MsjClass.setMsj ;
	nuevo->destruir = MsjClass.destruir ;
return nuevo;
}



struct Mensaje* MSJnewCon (unsigned int iIP, unsigned short shPuerto) {
	Mensaje* nuevo;
	nuevo = MsjClass.new();
	nuevo->header.msjId.iIP = iIP;
	nuevo->header.msjId.shPuerto = shPuerto;
return nuevo;
}

void setIpPuerto (unsigned int iIP, unsigned short shPuerto) {
	MsjClass.iIP = iIP;
	MsjClass.shPuerto = shPuerto;
return;
}

void (*______MSJDestruir) (Mensaje*);

void MsjClass_GoLive (unsigned int iIP, unsigned short shPuerto) {

	MsjClass.esIgualA = MSJesIgualA;
	MsjClass.recibirDe = MSJrecibirDe;
	MsjClass.enviarA = MSJenviarA;
	MsjClass.puedoRoutear = MSJpuedoRoutear;
	MsjClass.getSum = MSJgetSum;
	MsjClass.setTTL = MSJsetTTL;
	MsjClass.setHOPS = MSJsetHOPS;
	MsjClass.incHOPS = MSJincHOPS;
	MsjClass.setTipoMsj = MSJsetTipoMsj;
	MsjClass.setMsj = MSJsetMsj;
	
	______MSJDestruir = MSJdestruir;
	MsjClass.destruir = ______MSJDestruir;
	
	MsjClass.new = MSJnew;
	MsjClass.newCon = MSJnewCon;
	MsjClass.setIpPuerto = setIpPuerto;
	MsjClass.setIpPuerto (iIP,shPuerto);
	MsjClass.iSequence = 0;
return;
}


*/














