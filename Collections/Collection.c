#include "Collection.h"
#include <stdio.h>
/**
    El codigo solo esta probado sobre Linux con el compilador gcc
*/

/** Funciones Auxiliares. */
static int insertar (Collection* self, __ColNodo__* unNodo);
static bool __auxAddAll__ (Collection* self, void* unObjeto, Collection* unaColeccion) ;
static bool ComparaSt (void* self,void* unObjetoDeCol, void* unObjeto) ;
static bool __CuentaNodosColeccion__ (void* self, void* unObjetoDeCol, int* Rslt) ;

/** Métodos de clase  */

Collection* colNew (void) ;
void colClassGoLive (void);

/** Metodos de instancia */

	/** con efecto de lado */
	/** cualquier funcion / metodo de orden superior es colocado en esta
	    seccion debido a que es un potencial modificador de estado */

		int colAdd (Collection* self, void* unObjeto);
		int colAddAll (Collection* self, Collection* unaColeccion);
		Collection* colCollect(Collection* self, void* (*elaborador) (void* unObjetoDeCol));
		bool colDetect (Collection* self, bool (*Comparador) (void* self,void* unObjetoDeCol, void*unaClave), void* unaClave);
		Collection* colSelect (Collection* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave);
		void* colInyect (Collection* self, bool (*Into) (void* self, void*unObjetoDeCol, void*Rslt), void* Rslt);
		bool colDo (Collection* self, bool (*Algo) (Collection* self, void* unObjetoDeCol, void* unaClave), void* unaClave) ;
		bool colDoNot (Collection* self, bool (*Algo) (Collection* self, void* unObjetoDeCol, void* unaClave), void* unaClave);

	/** sin efecto de lado */
		bool colAllSatisfy (Collection* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave);
		bool colAnySatisfy (Collection* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave);
		int colCountCol (Collection* self) ;
		void* colGetFirst (Collection* self);
		void* colGetLast (Collection* self);
		void* colGetAnyOne (Collection* self);
		void* colGetOne (Collection*, bool(*) (void*,void*), void*);

/**
Lo mas importante de tener focalizado el lugar en que se manejan ciertas funcionalidades de nuestro sistema
es que, como todo nuestro sistema va estar utilizando el mismo fragmento de código vamos
a obtener una solidez mucho mayor, y una facilidad extra para cualquier modificacion, dado que
cuando encontremos un error solo hay que modificar un lugar de todo el codigo!

	Como se puede apreciar a simple vista, el manejo de punteros de funciones de
	esta manera y la gestion de datos / objetos en C es muy tediosa, su manejo es un
	tanto enredado al principio, y hay que estar atentos, pero a cambio de ello, el
	còdigo es mas sencillo y, una vez desarrollado el objeto,
	es mucho mas sencillo de manejar que un código estructurado, puesto que cada objeto
	sabe como hacer lo que se le pide.

	Una forma de ver el potencial de esta herramienta es que en estructurado se debe
	hacer por cada pila / cola / lista un codigo distinto, por estar vinculado con el
	tipo de dato. Desde este enfoque, en 200 lineas de código tienen suficiente material
	como para olvidarse de como se hacen las estructuras de este tipo :), que es el %40 de
	trabajo en un trabajo como el de Sistemas Operativos.



Para ver el ejemplo defina EJEMPLO
#define EJEMPLO
*/









/** ================================================================= */
/** Crea una nueva coleccion, es de metodo de clase 			  */
/** ================================================================= */

Collection* colNew (void) {
Collection* unaColeccion;

	unaColeccion = (Collection*)CALLOC (1, sizeof (__ColMensajes__));

	if (unaColeccion == NULL) {

		/* lanzar error */
	}
	unaColeccion->Mensaje = (CollectionClass.instanceMethods);
	unaColeccion->Cola = NULL;
	unaColeccion->Cabeza = NULL;
	unaColeccion->Actual = NULL;

return unaColeccion;
}

/** ================================================================= */
/** Inserta un nodo en una coleccion 						  */
/** ================================================================= */
static int insertar (Collection* self, __ColNodo__* unNodo) {
/*por defecto la insersion es como una cola, al final*/
	if (self->Cabeza == NULL) {
		self->Cabeza = unNodo;
	}
	if (self->Cola != NULL) {
		self->Cola->sgte = unNodo;
	}
	unNodo->sgte = NULL;
	unNodo->prev = self->Cola;

	self->Cola = unNodo;
return 0;
}



void colDestruir (Collection* self) {
__ColNodo__* aux;
	self->Actual = self->Cabeza;
	while (self->Actual != NULL) {
		aux = self->Actual->sgte;
		FREE (self->Actual);
		self->Actual = aux;
	}
return;
FREE (self);
}
/** ================================================================= */
/** crea un nodo y adjunta el objeto dado, luego lo inserta 		  */
/** ================================================================= */


int colAdd (Collection* self, void* unObjeto) {
__ColNodo__* unNodo;

	unNodo = CALLOC (1, sizeof(__ColNodo__));
	if (unNodo == NULL) {
		return -1;
		/* lanzar error */
	}
	unNodo->Objeto = unObjeto;
	return self->Mensaje->insertar (self, unNodo);
}



/** ================================================================= */
/** Inserta todos los nodos de una coleccion en otra			  */
/** ================================================================= */

int colAddAll (Collection* self, Collection* unaColeccion) {
	unaColeccion->Mensaje->Do (self,(bool (*)(void*,void*,void*)) __auxAddAll__, unaColeccion);
return 0;
}
/** auxiliar de insercion */
static bool __auxAddAll__ (Collection* self, void* unObjeto, Collection* unaColeccion) {
	unaColeccion->Mensaje->add (unaColeccion, unObjeto);
return TRUE;
}


/** ================================================================= */
/** Verifica que todos los nodos cumplan con una condicion dada 	  */
/** ================================================================= */

bool colAllSatisfy (Collection* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave) {
	return (self->Mensaje->Do (self, Condicion, unaClave));
}

/** ================================================================= */
/** Verifica que alguno de los nodos cumplan con una condicion dada 	  */
/** ================================================================= */

bool colAnySatisfy (Collection* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave){
	return (self->Mensaje->Do_not (self, Condicion, unaClave));
}

/** ================================================================= */
/** Crea una coleccion a partir de otra, pasando por una funcion de   */
/** elaboracion todos los objetos previamente  				  */
/** ================================================================= */

Collection* colCollect(Collection* self, void* (*elaborador) (void* unObjetoDeCol)) {
Collection* unaColeccion;
	unaColeccion = CollectionClass.new();

	self->Actual = self->Cabeza;

	while (self->Actual != NULL){
			unaColeccion->Mensaje->add (unaColeccion,elaborador (self->Actual->Objeto));
			self->Actual = self->Actual->sgte;
	}

return unaColeccion;
}


/** ================================================================= */
/** Hace pasar a cada objeto de la coleccion por una funcion dada, junto con
    la coleccion y una clave, mientras la funcion dada de TRUE 	  */
/** ================================================================= */


bool colDo (Collection* self, bool (*Algo) (Collection* self, void* unObjetoDeCol, void* unaClave), void* unaClave) {
bool Retorno = FALSE;
	self->Actual = self->Cabeza;

	while ((self->Actual != NULL) && (Retorno = Algo (self,(self->Actual->Objeto),unaClave)) == TRUE){
		self->Actual = self->Actual->sgte;
	}
return (self->Actual == NULL && Retorno) ;
}

bool subDo (Collection* self, bool (*Algo) (Collection* self, void* __ColNodo__, void* unaClave), void* unaClave) {
bool Retorno = FALSE;
	self->Actual = self->Cabeza;

	while ((self->Actual != NULL) && (Retorno = Algo (self,(self->Actual),unaClave)) == TRUE){
		self->Actual = self->Actual->sgte;
	}
return (self->Actual == NULL && Retorno) ;
}
/** ================================================================= */
/** Hace pasar a cada objeto de la coleccion por una funcion dada, junto con
    la coleccion y una clave, mientras la funcion dada de FALSE 	  */
/** ================================================================= */

bool colDoNot (Collection* self, bool (*Algo) (Collection* self, void* unObjetoDeCol, void* unaClave), void* unaClave) {
bool Retorno = FALSE;
	self->Actual = self->Cabeza;

	while ((self->Actual != NULL) && (Retorno = Algo (self,(self->Actual->Objeto),unaClave)) == FALSE){
		self->Actual = self->Actual->sgte;
	}
return (Retorno) ;
}

/** ================================================================= */
/** Revisa si existe el objeto en la coleccion, si el criterio se
    omite (NULL) se utiliza la comparacion de identidad			  */
/** ================================================================= */


bool colDetect (Collection* self, bool (*Comparador) (void* self,void* unObjetoDeCol, void*unaClave), void* unaClave) {
	return self->Mensaje->Do_not (self, ((Comparador == NULL)? ComparaSt:Comparador), unaClave);
}

/** auxiliar de comparacion */
static bool ComparaSt (void* self,void* unObjetoDeCol, void* unObjeto) {
	return (unObjetoDeCol != unObjeto);
}




/** ================================================================= */
/** Modifica un parametro en base a una funcion dada,
    el parametro debe ser inicializado previamente :( (una de las mayores
    desventajas de usar void es que void no es nada, por lo que no puedo
    ponerle nada a no ser que pida otra funcion por parametro para setear
    el valor inicial...)							  	  */
/** ================================================================= */

void* colInyect (Collection* self, bool (*Into) (void* self, void*unObjetoDeCol, void*Rslt), void* Rslt) {
	self->Mensaje->Do (self, Into, Rslt);
	return Rslt;
}

/** ================================================================= */
/** Cuanta la cantidad de objetos en la coleccion 			  */
/** ================================================================= */

int colCountCol (Collection* self) {
int Rslt = 0;
	self->Mensaje->inyect (self,(bool(*)(void*,void*,void*)) __CuentaNodosColeccion__, &Rslt);
return Rslt;
}
/** auxiliar de cuenta */
static bool __CuentaNodosColeccion__ (void* self, void* unObjetoDeCol, int* Rslt) {
	(*Rslt)++;
return TRUE;
}
/* por una cuestion de definicion recursiva, no voy a usar el mensaje do, sino que voy a
hacerlo a mano. Si quiciera hacerlo con el do tendria que tener sobrecarga de metodos,
que es algo que el C no proporciona*/


/** ================================================================= */
/** Crea una nueva coleccion a partir de una condicion de pertenencia */
/** ================================================================= */

Collection* colSelect (Collection* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave) {
Collection* unaColeccion;

	unaColeccion = CollectionClass.new();

	self->Actual = self->Cabeza;

	while (self->Actual != NULL){
		
		if (Condicion ((self->Actual->Objeto),unaClave) == TRUE) {
			unaColeccion->Mensaje->add (unaColeccion,self->Actual->Objeto);
		}
		self->Actual = self->Actual->sgte;
	}

return unaColeccion;
}

/** ================================================================= */
/** Devuelve el primer objeto de la coleccion 				  */
/** ================================================================= */

void* colGetFirst (Collection* self) {
	if (self->Cabeza != NULL)
		return self->Cabeza->Objeto;
return NULL;
}
/** ================================================================= */
/** Devuelve el ultimo objeto de la coleccion 				  */
/** ================================================================= */
void* colGetLast (Collection* self) {
	if (self->Cola != NULL)
		return self->Cola->Objeto;
return NULL;
}

/** ================================================================= */
/** Devuelve algun objeto de la coleccion 				  */
/** ================================================================= */
void* colGetAnyOne (Collection* self) {
	if (self->Actual != NULL)
		return self->Actual->Objeto;
	if (self->Cabeza != NULL)
		return self->Cabeza->Objeto;
return NULL;
}
/** ================================================================= */
/** Devuelve un objeto de la coleccion segun criterio 				  */
/** ================================================================= */

void* colGetOne (Collection* self, bool(*criterio) (void*,void*), void* clave) {
		self->Actual = self->Cabeza;
		while (self->Actual != NULL && !criterio (self->Actual->Objeto,clave)){
			self->Actual = self->Actual->sgte;
		}
		return self->Actual->Objeto;
}
/** ================================================================= */
/** Remueve un objeto de la coleccion (si se omite la funcion de 	*/
/** condicion se utiliza la igualdad de memoria) 				*/
/** ================================================================= */

bool esIgual (void* self, void* unObjetoDeCol, void* unaClave) {
	return (unObjetoDeCol == unaClave);
}

bool DestruirSi (Collection* self, void* clave,bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave)) {
__ColNodo__* obj;
bool Retorno = FALSE;
self->Actual = self->Cabeza;

while ((self->Actual != NULL)) {
	if (Condicion (self,(self->Actual->Objeto),clave) == TRUE){
		Retorno = TRUE;

		if (self->Actual->prev != NULL) 
			self->Actual->prev->sgte = self->Actual->sgte;

		if (self->Actual->sgte != NULL)
			self->Actual->sgte->prev = self->Actual->prev;

		if (self->Actual == self->Cabeza) self->Cabeza = self->Cabeza->sgte;
		if (self->Actual == self->Cola) self->Cola = self->Cola->prev;
		obj = self->Actual;
		self->Actual = self->Actual->sgte;
		FREE (obj);
	}
	else {
		self->Actual = self->Actual->sgte;
	}
}
return Retorno;
}

bool colRemove (Collection* self, void* clave, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave)) {
	return DestruirSi (self, clave, (Condicion? Condicion:esIgual));
}



/** ================================================================= */
/** Go live es una funcion que setea una variable global, es la forma
    en la que se le da vida a la clase y vincula mensajes con metodos
    para poder comenzar a crear objetos */
/** ================================================================= */
void colClassGoLive (void) {
	CollectionClass.new =(void*(*)(void)) colNew;

	CollectionClass.instanceMethods = CALLOC (1, sizeof(__ColMensajes__));
	if (CollectionClass.instanceMethods == NULL){ /* tirar error*/}

	CollectionClass.instanceMethods->add = (int(*)(void*,void*)) colAdd;
	CollectionClass.instanceMethods->addAll = (int(*)(void*,void*)) colAddAll;
	CollectionClass.instanceMethods->allSatisfy = (bool(*)(void*, bool(*)(void*,void*,void*),void*)) colAllSatisfy;
	CollectionClass.instanceMethods->anySatisfy = (bool(*)(void*, bool(*)(void*,void*,void*),void*))colAnySatisfy;

	CollectionClass.instanceMethods->collect = (void* (*) (void*, void*(*)(void*)))colCollect;
	CollectionClass.instanceMethods->detect = (bool(*)(void*,bool(*)(void*,void*,void*),void*)) colDetect;
	CollectionClass.instanceMethods->inyect = (void*(*)(void*, bool(*)(void*,void*,void*),void*))colInyect;
	CollectionClass.instanceMethods->select = (void*(*)(void*, bool (*) (void*,void*),void*)) colSelect;
	CollectionClass.instanceMethods->Do = (bool (*)(void*,bool(*)(void*,void*,void*), void*))colDo; /*este va en mayuscula por que es palabra reservada*/
	CollectionClass.instanceMethods->subDo = (bool (*)(void*,bool(*)(void*,__ColNodo__*,void*), void*))subDo; /*este va en mayuscula por que es palabra reservada*/
	CollectionClass.instanceMethods->Do_not = (bool (*)(void*,bool(*)(void*,void*,void*), void*))colDoNot;
 													 /*Do se ejecuta mientras la funcion devuelva TRUE DoNot se ejecuta mientras sea FALSE*/
	CollectionClass.instanceMethods->insertar = (int(*)(void*,__ColNodo__*))insertar;
	CollectionClass.instanceMethods->getFirst = (void*(*)(void*))colGetFirst;
	CollectionClass.instanceMethods->getLast = (void*(*)(void*))colGetLast;
	CollectionClass.instanceMethods->getAnyOne = (void*(*)(void*)) colGetAnyOne;
	CollectionClass.instanceMethods->getOne = (void*(*)(void*, bool(*)(void*,void*),void*) ) colGetOne;
	
	CollectionClass.instanceMethods->count = (int(*)(void*))colCountCol;

	CollectionClass.instanceMethods->remove = (bool (*)(void*,void*,bool(*)(void*,void*,void*)))colRemove;
	CollectionClass.instanceMethods->destruir = (void(*)(void*)) colDestruir;
}



























#ifdef EJEMPLO
#include <stdio.h>
#include <string.h>
	/** Ejemplo de uso!  */

	/**
		Al no existir bloques o funciones sin nombre, nos vemos obligados a hacer funciones
		de criterio.
	*/

bool printAll (Collection* self, int* unEntero) {
	printf ("El numero es %d\n",((unEntero==NULL)? -1:*unEntero));
return TRUE;
}
bool menorQue (int* unObjeto, int* unTope) {
	return (*unObjeto < *unTope);
}
char* Elaborador (int* unObjeto) {
char * retorno = CALLOC (1,20);
	sprintf (retorno, "%d(cadena)", *unObjeto);
return retorno;
}
bool printCharAll (Collection* self, char* unaCadena) {
	printf ("%s\n",unaCadena);
return TRUE;
}

bool ComparaInt (int* obj, int* obj2) {
	return (*obj == *obj2);
}

bool Condicion2 (Collection*self, int* unObjetoDeCol, int* unaClave){
	return (*unObjetoDeCol < *unaClave);

}

bool Into (Collection* self, char*unObjetoDeCol, char*Rslt){
	strcat (Rslt,unObjetoDeCol);
	strcat (Rslt,"|");
return TRUE;
}

int main (void) {
Collection* unaCol, *Seleccionada, *Colectada;
Collection* AddAll;
int* entero;
int Tope = 6;
int i;
char pepe [100];
/** Doy vida a la clase Coleccion*/
	colClassGoLive();

/** Creo una Coleccion por metodo de clase */
	unaCol = CollectionClass_New();

/** Como no tengo implementados mas objetos utilizo espacios de
    memoria para enteros para hacerlos de objetos.. C no tiene restricciones, pero claro
    el programador debe tenerla, tengan muy en cuenta que cualquier hibrido de conceptos
    puede ser una facilidad, puede ser mas poderoso y puede ser problemàtico, asi que,
    si no tienen opcion, asegurence de saber donde es que existen. */

	for (i = 0; i< 20; i++) {
		entero = CALLOC (1, sizeof(int));
		*entero = i;
		/** */
		unaCol->Mensaje->add (unaCol, entero);
	}

/**	Imprimo toda la coleccion (ejemplo de Do)*/
	unaCol->Mensaje->Do (unaCol, (bool(*)(void*,void*,void*)) printAll, NULL);
	unaCol->Mensaje->remove (unaCol, unaCol->Mensaje->getLast(unaCol), NULL);
	unaCol->Mensaje->remove (unaCol, unaCol->Mensaje->getFirst(unaCol), NULL);

	unaCol->Mensaje->Do (unaCol, (bool(*)(void*,void*,void*)) printAll, NULL);
/** 	Creo una coleccion con todos los objetos enteros que sean menores que "Tope" */
	Seleccionada = unaCol->Mensaje->select (unaCol, (bool(*)(void*,void*))menorQue,&Tope);

	


/** 	No cambia el tipo de dato de los objetos, puedo usar
	la misma funcion auxiliar con el mismo metodo para imprimir */

	Seleccionada->Mensaje->Do (Seleccionada, (bool(*)(void*,void*,void*)) printAll, NULL);

/** Creo una coleccion en base a una funcion que toma algunos objetos
    enteros y los transforma en objetos cadena*/

	Colectada = unaCol->Mensaje->collect (Seleccionada, (void*(*)(void*))Elaborador);

/** 	Para imprimir una coleccion de cadenas de caracteres solo debo cambiar la funcion auxiliar */

	Colectada->Mensaje->Do (Colectada, (bool(*)(void*,void*,void*)) printCharAll, NULL);

/**  Creo una nueva coleccion con la clase */

	AddAll = CollectionClass_New();

/**  Copio la primer coleccion, las dos colecciones van a tener los mismos valores, o sea
	el 9 que van a tener las dos colecciones es exactamente el mismo */

	unaCol->Mensaje->addAll (unaCol, AddAll);

/** Imprimo */
	AddAll->Mensaje->Do (AddAll, (bool(*)(void*,void*,void*)) printAll, NULL);
	printf ("Esta %d en addall? %s\n",*entero,(AddAll->Mensaje->detect (AddAll, NULL, entero))?"TRUE":"FALSE");

	*entero = 20;

/**como los enteros no estan definidos como objetos, no son inmutables,
	por lo que otro detect con NULL como funcion de comparacion (usa el ==)
	sin cambiar el objeto, va a dar verdadero, y la coleccion va a pasar de
	tener el numero 19 a tener el 20, por que usamos el mismo objeto o referencia*/

	printf ("Esta %d en addall? %s\n",*entero,(AddAll->Mensaje->detect (AddAll, NULL, entero))? "TRUE":"FALSE");
	AddAll->Mensaje->Do (AddAll, (bool(*)(void*,void*,void*)) printAll, NULL);
/** ahora, si yo pregunto por otro objeto, por ejemplo "i", no va a estar en la coleccion */
	printf ("Esta %d en addall? %s\n",i,(AddAll->Mensaje->detect (AddAll, NULL, &i))? "TRUE":"FALSE");

/** si cambio el criterio de comparaciòn (mensaje que deberia ser construido dentro de los objetos comparables),
	20 si esta! (el valor de i)*/
	printf ("Esta %d en addall? %s\n",i,(AddAll->Mensaje->detect (AddAll, (bool(*)(void*,void*))ComparaInt, &i))? "TRUE":"FALSE");

	Tope = 21;
/** Ahora pruebo el allSatisfy, */
	printf ("Todos son menores que 21? %s\n", (unaCol->Mensaje->allSatisfy (unaCol, (bool(*)(void*,void*, void*))Condicion2, &Tope))? "TRUE":"FALSE");
	Tope = 19;

	printf ("Todos son menores que 19? %s\n", (unaCol->Mensaje->allSatisfy (unaCol, (bool(*)(void*, void*,void*))Condicion2, &Tope))? "TRUE":"FALSE");

/** Any satisfy... */
	printf ("Hay algun numero menor que 19? %s\n", (unaCol->Mensaje->anySatisfy (unaCol, (bool(*)(void*, void*,void*))Condicion2, &Tope))? "TRUE":"FALSE");

/** Al mensaje Count le corresponde un metodo basado en inyect, obviamente se puede sobreescribir  */
	printf ("hay %d objetos en la coleccion\n",unaCol->Mensaje->count(unaCol));

/** Lamentablemente es muy poco practico hacer que el inyect inicialice el parametro que va a
    "inyectar", por lo que hay que inicializarlo previamente. */
	strcpy (pepe,"");
/** dado que el count muestra un inyect que solo cuenta, aca muestro uno que procesa, concatena todos
    los valores de la coleccion de cadenas en una sola variable*/
	Colectada->Mensaje->inyect (Colectada,(bool (*)(void*,void*,void*)) Into, pepe);


	printf ("inyect de caracteres :%s \n",pepe);

}


#endif











