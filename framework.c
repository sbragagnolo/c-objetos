#define _GNU_SOURCE
#define ___USE_GNU

#include "memLib/memManager.h"
#include "fileDescriptor/fd.h"
#include "fileDescriptor/file.h"
#include "eventLoger/logger.h"
#include "String/string.h"
#include "Hilo/ult.h"
#include "Hilo/mutex.h"
#include "Hilo/condition.h"
#include "Hilo/monitor.h"
#include "Hilo/datacontrol.h"
#include "Collections/Collection.h"
#include "Collections/Diccionario.h"
#include "Collections/DiccionarioHibrido.h"
#include "errMan/errobj.h"
#include "errMan/errman.h"
#include "Server/Server.h"
#include "Server/Mensajes.h"
#include "Automata/estado.h"
#include "Automata/automata.h"
#include "Automata/automan.h"


#define framework_GoLive() FdObjClass_GoLive (); \
							FileObjClass_GoLive (); \
							StringClass_GoLive (); \
							colClassGoLive (); \
							DiccionarioClass_GoLive (); \
							DicHibrid_GoLive (); \
							ultClassGoLive (); \
							EstadoClass_GoLive ();\
							Automata_GoLive (); \
							AutomanClass_GoLive (); \
							ServerClassGoLive (); \
							MutexClass_GoLive (); \
							ConditionClass_GoLive (); \
							MonitorClass_GoLive (); \
							DataControlClass_GoLive (); \
 						    LogClass_GoLive (); \
/*void framework_GoLive (void) {
	
	
}
*/

