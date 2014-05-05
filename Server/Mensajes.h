#ifndef MENSAJES_H_
#define MENSAJES_H_
#include "../fileDescriptor/fd.h"

typedef struct MsjHeader {
	struct __msjId {
		unsigned int iIP;
		unsigned short shPuerto;
		unsigned int iSeqLocal;
	}msjId;
	unsigned char cTipoMensaje;
	unsigned char cTTL;
	unsigned char cHOPS;
	unsigned int iLongMsj;
	unsigned int iSum;
} MsjHeader;

/**
	Mensaje es un objeto de manejo de mensajes estandar o basicos. Implementa una
	política doble de seguridad a la recepcion, por longitud y por sumatoria de 
	contenido. Se encuentra en una capa inferior 
*/
typedef struct Mensaje {
	MsjHeader header;
	void* pMensaje;
/*	bool (*esIgualA)(struct Mensaje*, struct Mensaje*); 
	int (*recibirDe) (struct Mensaje*,FdObj*);		
	int (*enviarA) (struct Mensaje*,FdObj*);
	bool (*puedoRoutear)(struct Mensaje*);
	unsigned int (*getSum) (struct Mensaje*);
	void (*setTTL) (struct Mensaje*, unsigned char);
	void (*setHOPS) (struct Mensaje*, unsigned char);
	void (*incHOPS) (struct Mensaje*);
	void (*setTipoMsj) (struct Mensaje*, unsigned char);
	void (*setMsj) (struct Mensaje*, void*, unsigned int);
	void (*destruir) (struct Mensaje*);*/
}Mensaje;
/*
typedef struct ___MsjClass___ {
	unsigned int iIP;
	unsigned short shPuerto;
	int iSequence;
	bool (*esIgualA)(struct Mensaje*, struct Mensaje*);
	int (*recibirDe) (struct Mensaje*,FdObj*);		
	int (*enviarA) (struct Mensaje*,FdObj*);
	bool (*puedoRoutear)(struct Mensaje*);
	unsigned int (*getSum) (struct Mensaje*);
	void (*setTTL) (struct Mensaje*, unsigned char);
	void (*setHOPS) (struct Mensaje*, unsigned char);
	void (*incHOPS) (struct Mensaje*);
	void (*setTipoMsj) (struct Mensaje*, unsigned char);
	void (*setMsj) (struct Mensaje*, void*, unsigned int);
	void (*destruir) (struct Mensaje*);
	void (*setIpPuerto) (unsigned int, unsigned short);
	struct Mensaje* (*new )(void);
	struct Mensaje* (*newCon )(unsigned int, unsigned short);
}___MsjClass___;

___MsjClass___ MsjClass;
*/
bool header_esIgualA (struct MsjHeader*, struct MsjHeader*);
int header_recibirDe (struct MsjHeader*,FdObj*);
struct MsjHeader* header_crearDesde (FdObj*);
bool header_puedoRoutear(struct MsjHeader*);
void header_setTTL (struct MsjHeader*, unsigned char);
void header_setHOPS (struct MsjHeader*, unsigned char);
void header_incHOPS (struct MsjHeader*);
void header_setTipoMsj (struct MsjHeader*, unsigned char);
void header_setLong (struct MsjHeader*, unsigned int);


MsjHeader* header_Crear (MsjHeader* nuevoHeader,unsigned int iIP, unsigned short shPuerto, unsigned char cTTL, unsigned char cHOPS, unsigned char cTipo, unsigned int iLongMsj);


#endif /*MENSAJES_H_*/
