#ifndef DATACONTROL_H_
#define DATACONTROL_H_
#include "mutex.h"
#include "condition.h"

typedef enum {
		TERMINAR = 666,
		HABILITADO = 1,
		DESHABILITADO = 2
} StdEstado;
typedef struct DataControl{
		MutexObj* mtx;
		Condicional* cnd;
		int iEstado;
		void (*bloquear) (struct DataControl*, Collection*);
		void (*desbloquear) (struct DataControl*, Collection*);
		void (*setearEstado)(struct DataControl*,int);
		int (*esperarEstado)(struct DataControl*);
		void (*destruir) (struct DataControl*, Collection*);
}DataControl; 

typedef struct __DataControlClass{
		void (*bloquear) (struct DataControl*, Collection*);
		void (*desbloquear) (struct DataControl*, Collection*);
		void (*setearEstado)(struct DataControl*,int);
		int (*esperarEstado)(struct DataControl*);
		void (*destruir) (struct DataControl*, Collection*);
		DataControl* (*new) (void);	
}__DataControlClass;

__DataControlClass DataControlClass;

void DataControlClass_GoLive (void); 


#endif /*DATACONTROL_H_*/
