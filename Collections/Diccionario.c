#define _GNU_SOURCE
#define __USE__GNU



#include <stdio.h>

#include "Diccionario.h"

/** para ver ejemplo de diccionario fijo: 

#define DICFIXEXAMPLE*/

/** 
	segmento de destruccion de objetos, por cuestion de permisos del 
procesador es obligatorio que el metodo de destruccion este en un lugar seguro, sino
vamos a estar ejecutando un segmento que va a ser borrado.
*/
void (*__Dic_destruir) (struct Diccionario*);

/** 

para insertar en una tabla de hash, la etiqueta de entrada debe ser una cadena de texto, para lograr poner un objeto de etiqueta lo que hacemos es poner como etiqueta de caracteres la direccion de memoria
del objeto, permitiendo utilizar el mismo objeto para recuperar el valor =) 

*/
char* objHashString (struct hasheable* self){ 
char strCode [CODELONG + 1];
char * sHashCode;
void * unObj = self->unObjeto;
	snprintf (strCode, CODELONG, "%p", unObj);
	sHashCode = CALLOC (strlen(strCode),sizeof(char));
	strcpy (sHashCode,strCode);
return sHashCode;
}

/**
	para las etiquetas es suficiente con alocar memoria, copiar la etiqueta y listo. es necesario que
	quede una copia por cuestiones de implementacion. 
*/
char* tagHashString (struct hasheable* self){
char* sRetorno;
char* Arg = (char*) self->unObjeto;
	sRetorno = CALLOC (1, strlen(Arg));
	strcpy (sRetorno, Arg);
return sRetorno;
}

/** inserta en un diccionario dados un tag y un obj */

int insertar_keyTag (struct Diccionario* self, char*tag, void*obj) {
hasheable Hasheable;
	Hasheable.hashString = tagHashString;
	Hasheable.unObjeto = tag;
	return self->insertar_Hasheable (self,&Hasheable,obj);

}

/** inserta en un diccionario dados un obj y un obj */

int insertar_keyObj (struct Diccionario* self, void* clave, void* obj){
hasheable Hasheable;
	Hasheable.hashString = objHashString;
	Hasheable.unObjeto = clave;
	return self->insertar_Hasheable (self,&Hasheable,obj);

}

/** inserta en un diccionario dada una estructura hasheable .*/

int insertar_Hasheable (struct Diccionario*self, hasheable*Hasheable, void* obj){
char* key;
	key = Hasheable->hashString(Hasheable);
	return self->insertar (self,key,obj);
}

/** inserta en un diccionario dados una cadena propia y un objeto. (insersion definitiva) */

int insertar (struct Diccionario* self, char* key, void* obj) {
ENTRY unPar, *otroPar;

	unPar.key = key;
	unPar.data = obj;
	
	if (!hsearch_r(unPar, ENTER,&otroPar,&(self->TablaHash))) {
	
		return HASH_FAIL;
	}
return 0;
}

/** buscar un objeto dado un tag */

void* buscar_keyTag (struct Diccionario* self, char* tag) {
hasheable Hasheable;
	Hasheable.hashString = tagHashString;
	Hasheable.unObjeto = tag;

	
return self->buscar_Hasheable (self,&Hasheable);
}

/** buscar un objeto dado otro objeto */

void* buscar_keyObj (struct Diccionario*self, void* unObj){
hasheable Hasheable;
	Hasheable.hashString = objHashString;
	Hasheable.unObjeto = unObj;

return self->buscar_Hasheable (self,&Hasheable);
}

/** buscar un objeto dada una estructura hasheable */

void* buscar_Hasheable (struct Diccionario*self, hasheable*Hasheable){
return self->buscar (self, Hasheable->hashString(Hasheable));
}

/** buscar un objeto dada una cadena propia */
void* buscar (struct Diccionario* self, char* clave) {
ENTRY unPar, *otroPar;
	unPar.key = clave;

	if (!hsearch_r(unPar, FIND,&otroPar,&(self->TablaHash))) {

		/* error! */
		return NULL;
		
	}
	return otroPar->data;
}

/** 
crea un nuevo diccionario de tamaño fijo. como esta basado en una implementacion de tabla de 
hash del S.O. (basado en funciones reentrantes) que impone ancho fijo, el diccionario tiene 
como restriccion el ancho fijo. 
*/
Diccionario* newDicFix (struct ClassDiccionario* unaClase, int unSize) {
Diccionario* unDic;
	if (!(unDic = CALLOC (1,sizeof (Diccionario)))) {
		/* error!!*/
		return NULL;
	}
	unDic->insertar_keyTag = unaClase->insertar_keyTag ;
	unDic->insertar_keyObj = unaClase->insertar_keyObj ;
	unDic->insertar_Hasheable = unaClase->insertar_Hasheable ;
	unDic->insertar = unaClase->insertar ;
	unDic->buscar_keyTag = unaClase->buscar_keyTag ;
	unDic->buscar_keyObj = unaClase->buscar_keyObj ;
	unDic->buscar_Hasheable = unaClase->buscar_Hasheable ;
	unDic->buscar = unaClase->buscar ;
	unDic->destruir= unaClase->destruir ;
	memset (&(unDic->TablaHash), 0, sizeof (hsearch_data));
	hcreate_r (unSize,&(unDic->TablaHash));
return unDic;
}

/** 
libera recursos de hash y libera el objeto 

*/
void dicDestruir (struct Diccionario* self) {
	hdestroy_r (&(self->TablaHash));
	FREE (self);
}

/**
	inicializa la clase. 
*/

void DiccionarioClass_GoLive (void){

	__Dic_destruir = dicDestruir;
	DictionaryClass.destruir = __Dic_destruir ;
	DictionaryClass.insertar_keyTag = insertar_keyTag ;
	DictionaryClass.insertar_keyObj = insertar_keyObj ;
	DictionaryClass.insertar_Hasheable = insertar_Hasheable ;
	DictionaryClass.insertar = insertar ;
	DictionaryClass.buscar_keyTag = buscar_keyTag ;
	DictionaryClass.buscar_keyObj = buscar_keyObj ;
	DictionaryClass.buscar_Hasheable = buscar_Hasheable ;
	DictionaryClass.buscar = buscar ;
	DictionaryClass.new = newDicFix;
return;
}










































#ifdef DICFIXEXAMPLE

char *data[]=
{
               "alpha", "balpha", "calpha", "dalpha",
               "ealpha", "falpha", "galpha", "halpha", "ialpha", "alphaw",
               "kilo", "lima", "mike", "november", "oscar", "papa",
               "quebec", "romeo", "sierra", "tango", "uniform",
               "victor", "whiskey", "x-ray", "yankee", "zulu"
};


int main (void) {
int i;
char *s;
Diccionario* unDiccionario, *otro;
DiccionarioClass_GoLive ();

	unDiccionario = DictionaryClass.new (&DictionaryClass,30);
	otro = DictionaryClass.new (&DictionaryClass,30);
	unDiccionario->insertar_keyTag (unDiccionario, "x-ray",(void*)3);
	otro->insertar_keyTag (otro, "x-ray",(void*)20);


	printf("%9.9s ->:%d\n", "x-ray",
	(int)unDiccionario->buscar_keyTag (unDiccionario, "x-ray"));

	printf("%9.9s ->:%d\n", "x-ray",
	(int)otro->buscar_keyTag (otro, "x-ray"));


	unDiccionario->insertar_keyTag (unDiccionario, "x-ray",(void*)20);

	otro->insertar_keyTag (otro, "x-ray",(void*)3);



	printf("%9.9s ->:%d\n", "x-ray",
	(int)unDiccionario->buscar_keyTag (unDiccionario, "x-ray"));

	printf("%9.9s ->:%d\n", "x-ray",
	(int)otro->buscar_keyTag (otro, "x-ray"));


}

#endif












