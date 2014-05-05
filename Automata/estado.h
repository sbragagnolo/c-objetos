#define _GNU_SOURCE
#define ___USE_GNU

#ifndef ESTADOH_
#define ESTADOH_
#include "../Collections/Collection.h"
#include "../String/string.h"
#include "../errMan/errman.h"
#include "../memLib/memManager.h"
#include <unistd.h>


/**
 * Estados de Autómata:
 * 	un automata es una estructura conformada por estados, donde cada en cada 
 *  estado, el automata, acepta un juego de vocablos distinto, determinando asi
 * 	una forma prevista de combinacion de palabras, que quieren decir algo.
 * 	
 * 	El modelo de Estado incorpora:
 * 	Atributos:
 * 		ID:			ID de estado, forma de reconocer un estado como único
 * 		Vocablos:	Set de palabras aceptadas por el estado.
 * 		Tipo:		Tipo de estado.
 * 	Métodos:
 * 		compararCon:	Compara un estado con self, en funcion del ID
 * 		compararCon_i:	Compara un entero ID con el ID de self
 * 		getEstadoId:	Retorna el ID de un estado
 * 		agregarVocablo:	Agrega una palabra en el conjunto del estado.
 * 		removerVocablo:	Elimina una palabra del conjunto del estado.
 * 		esEsperado:		Indica si la palabra pertenece al conjunto del estado.
 * 		getTipo:		Indica el tipo de estado.
 * 		destruir:		Libera TODOS los recursos del estado. (incluye las cadenas)	
 * 	
 * 
 **/
typedef enum TipoEstado{
		FINAL   = 	0,
		INICIAL = 	1,
		INI_FIN =	2,
		NORMAL  =	3
}TipoEstado;

typedef struct Estado {
		int iEstadoId;
		Collection* Vocablos;
		TipoEstado Tipo;
		bool (*compararCon) (struct Estado*, struct Estado*);
		bool (*compararCon_i) (struct Estado*, int estado_id);
		int (*getEstadoId) (struct Estado*);
		void (*agregarVocablo) (struct Estado*, String*, int );
		void (*removerVocablo) (struct Estado*, String*);
		int (*getSgteEstado) (struct Estado*, String*);
		TipoEstado (*getTipo) (struct Estado*);
		void (*imprimirEsperados) (struct Estado*);
		void (*destruir) (struct Estado*);
}Estado;

typedef struct ParLexico {
		String* unaPalabra;
		int iEstadoSgte;
}ParLexico;


/**
 *  La clase de Estado incorpora un método constructor:
 *  
 * 		new (ID, TIPO) que genera un estado codigo ID y del tipo TIPO.
 * 			 
 *  
 **/
typedef struct ___ClaseEstado___ {
		bool (*compararCon) (struct Estado*, struct Estado*);
		bool (*compararCon_i) (struct Estado*, int estado_id);
		int (*getEstadoId) (struct Estado*);
		void (*agregarVocablo) (struct Estado*, String*, int);
		void (*removerVocablo) (struct Estado*, String*);
		int (*getSgteEstado) (struct Estado*, String*);
		TipoEstado (*getTipo) (struct Estado*);
		void (*imprimirEsperados) (struct Estado*);
		void (*destruir) (struct Estado*);
		Estado* (*new) (int, TipoEstado);
}___ClaseEstado___;

___ClaseEstado___ EstadoClass;
void EstadoClass_GoLive (void);

#endif /*ESTADOH_*/



