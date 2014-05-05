#ifndef AUTOMATA_H_
#define AUTOMATA_H_
#include "../Collections/Diccionario.h"
#include "../Collections/Collection.h"
#include "../String/string.h"
#include "../fileDescriptor/file.h"
#include "estado.h" 
#include <ctype.h>
/**
 *	El automata es una maquina de estados que analiza expresiones regulares.
 *  
 * 	En este caso, el automata va a reconocer un lenguaje que sera seteado  
 *  	(por seters/geters y/o archivo de conf)
 * 
 *  El mismo dara como resultado un tipo atomoExe, esta estructura es un juego
 * 		de información suficiente como para poder ejecutar un comando que desencadene 
 *  	un evento o bien que modifique alguna configuracion del sistema.  
 *    
 **/

/**
 * 
 *	Cada funcion en hash tiene aparejado un primer parametro potencial. con esto se busca aumentar mucho la 
 *  flexibilidad, permitiendo que cada funcion se pueda aplicar sobre distintos dominios. 
 *  
 **/
 
 	typedef struct onHashfunction {
			String* (*fnComando) (void* dominio, int, String**);
			void* fstParam;
	}OHF;
	
/**
 * 
 * atomoExe es la unidad de ejecución, contiene un tipo onHashFunc donde tiene dominio y funcion y tiene
 * a su vez (en base a los parametros dados por el usuario) una cantidad de parametros, y los parametros 
 * propiamente dichos.
 *  
 **/
	typedef struct atomoExe {
			OHF* Com;
			int argc;
			String** argv;		
	}atomoExe;


/**
 * 	Estados: Estados de los que dispone el automata
 * 	fnDir: Directorio de funciones.
 * 	sDelimSet: Caracteres delimitadores de instrucción. 
 * 	
 **/	
	typedef struct Automata {
			Collection* Estados;
			Diccionario* fnDir;
			String* sDelimSet;
			String* sSepSet;
			void(*agregarEstado) (struct Automata*, int, TipoEstado);
			void(*agregarVocabloEn) (struct Automata*,int,  String*, int sgte);
			void(*agregar_fn) (struct Automata*, String*(*)(void*,int,String**), int codigo, void* fstParam);
			void(*setDelimSet) (struct Automata*, String*);
			void(*setSepSet) (struct Automata*, String*);
			atomoExe*(*parsearString) (struct Automata*,String*);
			atomoExe*(*parsearLinea) (struct Automata*,char*);
			Collection*(*parsearArchivo) (struct Automata*,char*);
			Estado* (*getOne) (struct Automata*, int);
			OHF* (*getCommand)(struct Automata*, int);
			void (*destruir) (struct Automata*);
	}Automata;


/**
 *	La clase incorpora dos constructores, 
 *  uno (new) que genera un automata vacio para inicializar a mano, y
 *  otro (newCon) que genera un automata configurado por archivo.  
 *  
 **/
	typedef struct __AutomataClass__ {
			void(*agregarEstado) (struct Automata*, int, TipoEstado);
			void(*agregarVocabloEn) (struct Automata*,int ecod,  String*, int sgte);
			void(*agregar_fn) (struct Automata*, String*(*)(void*,int,String**), int codigo, void* fstParam);
			void(*setDelimSet) (struct Automata*, String*);
			void(*setSepSet) (struct Automata*, String*);
			atomoExe*(*parsearString) (struct Automata*,String*);
			atomoExe*(*parsearLinea) (struct Automata*,char*);
			Collection*(*parsearArchivo) (struct Automata*,char*);
			Estado* (*getOne) (struct Automata*,int);
			OHF* (*getCommand)(struct Automata*, int);
			void (*destruir) (struct Automata*);			
			Automata* (*new) (void);
			Automata* (*newCon) (char*);
	}__AutomataClass__;

__AutomataClass__ AutomataClass;

void Automata_GoLive (void);


/** 
 * Constantes lexicas para el generador de automatas .
 * */
#define COMMENT "--"
#define COMLONG 2

#define ENTERCOM "enter"
#define ENTERVAL "\n"

#define PIPECOM "pipe"
#define PIPEVAL "|"

#define PPCOM "pp"
#define PPVAL ":"

#define BLANKCOM "blanco"
#define BLANKVAL " "

#define COMACOM "coma"
#define COMAVAL ","

#define FINALCOM "final"
#define INICIALCOM "inicial"
#define NORMALCOM "normal"

void liberarArgs (int argc, String ** argv );
void to_lower (String* linea);
#endif /*AUTOMATA_H_*/




