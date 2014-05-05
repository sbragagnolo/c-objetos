#ifndef MONITOR_H_
#define MONITOR_H_
#include "mutex.h"
#include "condition.h"


typedef struct Monitor {
		MutexObj* mtx;
		Condicional** cnds;
		int iEstados;
		void (*bloquear) (struct Monitor*, Collection*);
		void (*desbloquear) (struct Monitor*,Collection*);
		void (*esperarEstado) (struct Monitor*, int iEstado);
		void (*senializarEstado) (struct Monitor*, int iEstado, Collection*);
		void (*destruir) (struct Monitor*, Collection*);		
}Monitor;   

typedef struct __MonitorClass {
		void (*bloquear) (struct Monitor*, Collection*);
		void (*desbloquear) (struct Monitor*,Collection*);
		void (*esperarEstado) (struct Monitor*, int iEstado);
		void (*senializarEstado) (struct Monitor*, int iEstado, Collection*);
		void (*destruir) (struct Monitor*, Collection*);		
		Monitor* (*new) (int);
					
}__MonitorClass;
__MonitorClass MonitorClass;

void MonitorClass_GoLive (void);

#endif /*MONITOR_H_*/
