#ifndef AUTOMAN_H_
#define AUTOMAN_H_
#include "automata.h"
#include "../Server/Server.h"
#include "../Hilo/ult.h"
#include "../Hilo/datacontrol.h"
#include "../Collections/Collection.h"
#include "../String/string.h"


/**
 * 
 * Automan es un manager de conexiones las cuales parsea, 
 * 	planifica y ejecuta. Posee una interfaz para que el 
 *  usuario pueda sumninistrar un archivo de configuración 
 *  de automata y pueda setear filedescriptors y/o archivos, funciones
 *  para el directorio. No permite el seteo manual de vocablos y/o estados
 *  con el fin de que el comportamiento del programa sea natural.  
 *  Basa su servicio en los objetos ULThread y Automata.
 *
 *  Por la caracteriztica de Servicio UNICO por proceso, se trata de una 
 *  clase inicialziable no instanciable. 
 **/



typedef struct Automan {
	Automata* Frontend;
	Collection * Clientes;
	ULThread Manager;
	String* nombre;
	DataControl* dc;
	void (*cargarConf)(struct Automan*,char*);
	void (*nuevoCliente) (struct Automan*,___client*); /*ver necesidades del servidor*/
	Collection* (*getActivos) (struct Automan*); /*  */
	void (*iniciar)(struct Automan*);
	void (*agregarFn)(struct Automan*, String*(*)(void*,int,String**), void*, int);
	void (*destruir)(struct Automan*);
}Automan;  

typedef struct _AutomanClass { 
	void (*cargarConf)(struct Automan*,char*);
	void (*nuevoCliente) (struct Automan*, ___client*); /*ver necesidades del servidor*/
	Collection* (*getActivos) (struct Automan*); /*  */
	void (*iniciar)(struct Automan*);
	void (*agregarFn)(struct Automan*, String*(*)(void*,int,String**), void*, int);
	void (*destruir)(struct Automan*);
	Automan* (*new)(void);
	Automan* (*newCon)(String*);
}_AutomanClass;

_AutomanClass AutomanClass;

void AutomanClass_GoLive (void);

 /** COMANDOS DE AUTOMAN. 
  * 	debido a la restriccion del automata a soportar comandos de un solo estado, el automan soporta comandos rapidos
  * 	de un solo caracter. Estos son definidos dentro de un case (a diferencia del automata que es una tabla de hash)
  * 	al ser un juego reducido, esta forma de ejecución no trae problemas de performance. 
  * */
#define SO_OUT ':'	/** luego de poner ":" introducir el comando de sistema operativo que desee ejecutar */
#define CLEAR '!'   /** ! limpia la pantalla */
#define EMPTY '\n'  /** esto significa "linea vacia" por lo que no se pasa al automata, simplemente se ignora.*/
#define SLASH '\\' /** luego de poner \ se sale del programa. En estos momentos sale con exit (0). ver 
						por salida limpia */
#define ARCH_LOAD '<' 
#define DUMP '>'
void to_lower_c (char* line) ;
void cargarArchivo (Automan* self, char* buffer, Cliente cl);
#endif /*AUTOMAN_H_*/









