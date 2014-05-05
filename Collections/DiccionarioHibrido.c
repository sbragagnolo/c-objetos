
#include "DiccionarioHibrido.h"
#include <stdio.h>
#define MINVAL 5


/** dicpath*/

/** insersiones como en una tabla de hash. */
int insertar_keyTagHD (struct DiccionarioHibrido* self, char* Tag, void* Obj) {
hasheable Hasheable;
	Hasheable.hashString = tagHashString;
	Hasheable.unObjeto = Tag;
	return self->insertar_Hasheable (self,&Hasheable,Obj);
}

int insertar_keyObjHD (struct DiccionarioHibrido* self, void* clave, void* Obj) {
hasheable Hasheable;
	Hasheable.hashString = objHashString;
	Hasheable.unObjeto = clave;
	return self->insertar_Hasheable (self,&Hasheable,Obj);
}

int insertar_HasheableHD (struct DiccionarioHibrido* self, hasheable* Hasheable, void* Obj) {
char* key;
	key = Hasheable->hashString(Hasheable);
	return self->insertar (self,key,Obj);
}

int insertarHD (struct DiccionarioHibrido* self, char* Tag, void* obj) {
__DicNodo__ * unDicNodo;
/**	Verifico si la insercion es una actualizacion, de ser asi, 
	solo actualizo y salgo*/		
		unDicNodo = self->unDiccionario->buscar_keyTag (self->unDiccionario,Tag);

		if (unDicNodo != NULL) {
			unDicNodo->Objeto = obj;

/** si se esta reutilizando un nodo que fue marcado como borrado y 
todavia existe, actualizo informacion. */
			if (unDicNodo->Marcado == BORRADO) {
				self->entradasEliminadas--;
				unDicNodo->Marcado = REUTILIZADO;
				if (self->entradasEliminadas < 0) 
					self->entradasEliminadas = 0;
			}
			return 0;
		}

/**	Creo un nodo de diccionario (obj para coleccion) */		
		unDicNodo = CALLOC (1, sizeof(__DicNodo__));
		unDicNodo->Tag = Tag;
		unDicNodo->Objeto = obj;
		unDicNodo->Marcado = EN_USO;

/**  actualizo informacion de management de diccionario */
		self->entradasHechas ++;
		self->entradasReales = self->entradasHechas - self->entradasEliminadas;

/** si queda poco espacio extiendo la estructura y reconstruyo */
		if (self->entradasHechas >= self->tamanioActual - self->puntoIncremento){
			self->extender (self, self->tamanioActual * (self->pctIncremento + 1));
		}
/** si llegue al umbral de nodos eliminados reconstruyo */

		if (self->entradasEliminadas/(self->entradasHechas+1) >= self->pctRecontruccion){
			self->reBuild (self);
		}

/** listo, agrego al diccionario, si va bien solo tengo que 
agregarlo tambien a la coleccion.. de tener problemas se puede ver de 
agregarlo primero a la coleccion y si la insercion falla por falta de espacio
en hash redimensionar sin perder el valor. 
*/
		if (self->unDiccionario->insertar (self->unDiccionario,
						Tag,
						unDicNodo)) {

			self->unaColeccion->Mensaje->add (self->unaColeccion, unDicNodo);
			
			return 0;
		}

return HASH_FAIL;
}

void* buscar_keyTagHD (struct DiccionarioHibrido* self, char* tag) {
__DicNodo__* Retorno;
	Retorno = self->unDiccionario->buscar_keyTag (self->unDiccionario,tag);
	return (Retorno == NULL)? NULL:Retorno->Objeto;
}

void* buscar_keyObjHD (struct DiccionarioHibrido*self , void* obj) {
__DicNodo__* Retorno;
	Retorno = self->unDiccionario->buscar_keyObj (self->unDiccionario,obj);
	return (Retorno == NULL)? NULL:Retorno->Objeto;
}

void* buscar_HasheableHD (struct DiccionarioHibrido* self, hasheable* Hasheable) {
__DicNodo__* Retorno;
	Retorno = self->unDiccionario->buscar_Hasheable (self->unDiccionario,Hasheable);
	return (Retorno == NULL)? NULL:Retorno->Objeto;

}
void* buscarHD (struct DiccionarioHibrido* self, char* tag) {
__DicNodo__* Retorno;
	Retorno = self->unDiccionario->buscar (self->unDiccionario,tag);
	return (Retorno == NULL)? NULL:Retorno->Objeto;

}






/** colpath*/
void* elaboradorHD (void* unObjetoDeCol) {
__DicNodo__* unObjeto = (__DicNodo__*) unObjetoDeCol;
	return unObjeto->Objeto;
}

bool allSatisfyHD(struct DiccionarioHibrido* self, bool (*Condicion) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave){
Collection* unaCol;
bool Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->allSatisfy (unaCol, Condicion, unaClave);
	unaCol->Mensaje->destruir (unaCol);

return Retorno;
}

bool anySatisfyHD (struct DiccionarioHibrido* self, bool (*Condicion) (void*self, void* unObjetoDeCol, void* unaClave), void* unaClave){
Collection* unaCol;
bool Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->anySatisfy (unaCol, Condicion, unaClave);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;
}

void* collectHD (struct DiccionarioHibrido* self, void* (*elaborador) (void* unObjetoDeCol)){
Collection* unaCol;
Collection* Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->collect(unaCol, elaborador);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;
}

bool detectHD (struct DiccionarioHibrido* self, bool (*Comparador) (void* unaCol, void* unObjetoDeCol, void*unaClave), void* unaClave){
Collection* unaCol;
bool Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->detect (unaCol, Comparador, unaClave);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;
}
void* inyectHD (struct DiccionarioHibrido* self, bool (*Into) (void*self, void*unObjetoDeCol, void*Rslt), void* Rslt){
Collection* unaCol;
void* Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->inyect(unaCol, Into, Rslt);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;
}

void* selectHD (struct DiccionarioHibrido* self, bool (*Condicion) (void* unObjetoDeCol, void* unaClave), void* unaClave){
Collection* unaCol;
Collection* Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->select(unaCol, Condicion, unaClave);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;

}
bool DoHD (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave){
Collection* unaCol;
bool Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->Do (unaCol, Algo, unaClave);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;

}
bool Do_notHD (struct DiccionarioHibrido* self, bool (*Algo) (void* self, void* unObjetoDeCol, void* unaClave), void* unaClave){
Collection* unaCol;
bool Retorno;
	unaCol = self->unaColeccion->Mensaje->collect (self->unaColeccion, elaboradorHD);
	Retorno = unaCol->Mensaje->Do_not (unaCol, Algo, unaClave);
	unaCol->Mensaje->destruir (unaCol);
return Retorno;
}
int countHD(struct DiccionarioHibrido* self) {
	return self->entradasReales;
}





void setPctRebuild (struct DiccionarioHibrido* self, int Pct) {
	self->pctRecontruccion = Pct;
}
void setPuntoIncremento (struct DiccionarioHibrido* self, int ptInc){
	self->puntoIncremento = ptInc;
}

void setPuntoDecremento (struct DiccionarioHibrido* self, int ptDec) {
	self->puntoDecremento = ptDec;
}
void setPctIncremento (struct DiccionarioHibrido* self, int pctInc) {
	self->pctIncremento = pctInc;
}

void remover_keyTag (struct DiccionarioHibrido* self, char* tag) {
hasheable Hasheable;
	Hasheable.hashString = tagHashString;
	Hasheable.unObjeto = tag;
	self->remover_Hasheable (self,&Hasheable);

}
void remover_keyObj (struct DiccionarioHibrido* self, void* Obj) {
hasheable Hasheable;
	Hasheable.hashString = objHashString;
	Hasheable.unObjeto = Obj;
	self->remover_Hasheable (self,&Hasheable);
}

void remover_Hasheable (struct DiccionarioHibrido* self, hasheable* Hasheable) {
char* key;
	key = Hasheable->hashString(Hasheable);
	self->remover (self,key );
}

bool esIgualChar (__DicNodo__* unObjetoDeCol, char* unaClave) {
	return !strcmp(unObjetoDeCol->Tag,unaClave);
}

bool MarcarBorrado (void* self, __DicNodo__* unObjetoDeCol, void* unaClave) {
	unObjetoDeCol->Marcado = BORRADO;
	unObjetoDeCol->Objeto = NULL;
return TRUE;
}

void remover (struct DiccionarioHibrido* self, char* tag) {
Collection* unaCol;

	unaCol = self->unaColeccion->Mensaje->select (self->unaColeccion,(bool(*)(void*,void*))esIgualChar,tag);
	unaCol->Mensaje->Do (unaCol, (bool(*)(void*,void*,void*))MarcarBorrado, NULL);

	self->entradasEliminadas +=unaCol->Mensaje->count (unaCol);
	unaCol->Mensaje->destruir (unaCol);
	if (((float)self->entradasEliminadas/(self->entradasHechas+1)) >= self->pctRecontruccion){
		self->reBuild (self);
	}

}


void (*_____destruirHD) (struct DiccionarioHibrido* self);

void destruirHD (struct DiccionarioHibrido* self) {
	self->unDiccionario->destruir (self->unDiccionario);
	self->unaColeccion->Mensaje->destruir (self->unaColeccion);
FREE (self);
}


void extender (struct DiccionarioHibrido* self, int iNuevo){
	self->tamanioActual = (self->tamanioActual < iNuevo)? iNuevo:self->tamanioActual;
	self->reBuild(self);
}

bool noBorrado (void* param, void* clave) {
__DicNodo__* unDicNodo = (__DicNodo__*) param;

	return (unDicNodo->Marcado != BORRADO);
}

bool reInsertar (Collection* self, __DicNodo__* unDicNodo, Diccionario* unDic) {
	unDic->insertar (unDic, unDicNodo->Tag,unDicNodo);
	unDicNodo->Marcado = EN_USO;
return TRUE;
}


void reBuild (struct DiccionarioHibrido* self){
Diccionario* unDic; 
Collection* unaCol, *Coll;
	unDic = DictionaryClass.new (&DictionaryClass,self->tamanioActual);
	

	Coll = self->unaColeccion;



	unaCol = Coll->Mensaje->select (Coll,noBorrado, NULL);


	unaCol->Mensaje->Do (unaCol, (bool(*)(void*, void*,void*))reInsertar, unDic);
	self->unaColeccion->Mensaje->destruir (self->unaColeccion);
	self->unDiccionario->destruir (self->unDiccionario);
	self->unaColeccion = unaCol;
	self->unDiccionario = unDic;
	self->entradasHechas = self->entradasReales;
	self->entradasEliminadas = 0;


}








DiccionarioHibrido* DicHibridNew (struct HDCLass* unaClase, int Tamanio) {
DiccionarioHibrido* unDic;
	unDic = CALLOC (1, sizeof (DiccionarioHibrido));

	unDic->insertar_keyTag = unaClase->insertar_keyTag ;
	unDic->insertar_keyObj = unaClase->insertar_keyObj ;
	unDic->insertar_Hasheable = unaClase->insertar_Hasheable ;
	unDic->insertar = unaClase->insertar ;
	unDic->buscar_keyTag = unaClase->buscar_keyTag ;
	unDic->buscar_keyObj = unaClase->buscar_keyObj ;
	unDic->buscar_Hasheable = unaClase->buscar_Hasheable ;
	unDic->buscar = unaClase->buscar ;


	unDic->allSatisfy = unaClase->allSatisfy ;
	unDic->anySatisfy = unaClase->anySatisfy ;
	unDic->collect = unaClase->collect ;
	unDic->detect = unaClase->detect ;
	unDic->inyect = unaClase->inyect ;
	unDic->select = unaClase->select ;
	unDic->Do = unaClase->Do ;
	unDic->Do_not = unaClase->Do_not ;
	unDic->count = unaClase->count;


	unDic->extender = unaClase->extender ;
	unDic->reBuild = unaClase->reBuild ;
	unDic->setPctRebuild = unaClase->setPctRebuild ;
	unDic->setPuntoIncremento = unaClase->setPuntoIncremento ;
	unDic->setPuntoDecremento = unaClase->setPuntoDecremento ;
	unDic->setPctIncremento = unaClase->setPctIncremento ;
	unDic->destruir = unaClase->destruir ;
	unDic->remover_keyTag = unaClase->remover_keyTag ;
	unDic->remover_keyObj = unaClase->remover_keyObj ;
	unDic->remover_Hasheable = unaClase->remover_Hasheable ;
	unDic->remover = unaClase->remover;

	unDic->pctIncremento = 0.25;	/* crece en un 25 por ciento*/
	unDic->puntoIncremento = 2;	/* crece cuando solo sobran 2 entradas*/
	unDic->puntoDecremento = 0;	/** no esta en uso todavia.*/
	unDic->pctRecontruccion = 0.1; /* permite %10 de sobrecarga*/
	Tamanio = (Tamanio >= MINVAL)? Tamanio:MINVAL;
	unDic->tamanioActual = Tamanio ;

	unDic->unDiccionario = DictionaryClass.new (&DictionaryClass,Tamanio);
	unDic->unaColeccion = CollectionClass.new();

return unDic;
}



void DicHibrid_GoLive (void)  {
	 colClassGoLive (); 
	 DiccionarioClass_GoLive (); 
	__DicHibridClass.insertar_keyTag = insertar_keyTagHD ;
	__DicHibridClass.insertar_keyObj = insertar_keyObjHD ;
	__DicHibridClass.insertar_Hasheable = insertar_HasheableHD ;
	__DicHibridClass.insertar = insertarHD ;
	__DicHibridClass.buscar_keyTag = buscar_keyTagHD ;
	__DicHibridClass.buscar_keyObj = buscar_keyObjHD ;
	__DicHibridClass.buscar_Hasheable = buscar_HasheableHD ;
	__DicHibridClass.buscar = buscarHD ;


	__DicHibridClass.allSatisfy = allSatisfyHD ;
	__DicHibridClass.anySatisfy = anySatisfyHD ;
	__DicHibridClass.collect = collectHD ;
	__DicHibridClass.detect = detectHD ;
	__DicHibridClass.inyect = inyectHD ;
	__DicHibridClass.select = selectHD ;
	__DicHibridClass.Do = DoHD ;
	__DicHibridClass.Do_not = Do_notHD ;
	__DicHibridClass.count = countHD;


	__DicHibridClass.extender = extender ;
	__DicHibridClass.reBuild = reBuild ;
	__DicHibridClass.setPctRebuild = setPctRebuild ;
	__DicHibridClass.setPuntoIncremento = setPuntoIncremento ;
	__DicHibridClass.setPuntoDecremento = setPuntoDecremento ;
	__DicHibridClass.setPctIncremento = setPctIncremento ;
	_____destruirHD  = destruirHD;
	__DicHibridClass.destruir = _____destruirHD;
	__DicHibridClass.remover_keyTag = remover_keyTag ;
	__DicHibridClass.remover_keyObj = remover_keyObj ;
	__DicHibridClass.remover_Hasheable = remover_Hasheable ;
	__DicHibridClass.remover = remover;
	__DicHibridClass.new = DicHibridNew;


}













#ifdef DICHIBEXAMPLE

char *data[]=
{
               "alpha", "balpha", "calpha", "dalpha",
               "ealpha", "falpha", "galpha", "halpha", "ialpha", "alphaw",
               "kilo", "lima", "mike", "november", "oscar", "papa",
               "quebec", "romeo", "sierra", "tango", "uniform",
               "victor", "whiskey", "x-ray", "yankee", "zulu"
};


bool printAll (Collection* self, int unNodo, void* unClave) {
	printf ("El numero es %d\n",unNodo);
return TRUE;
}

int main (void) {
int i,j;
char *s;
DiccionarioHibrido* unDiccionario, *otro;
DicHibrid_GoLive ();

	unDiccionario = __DicHibridClass.new (&__DicHibridClass,3);
	otro = __DicHibridClass.new (&__DicHibridClass,30);


	for (i = 0; i< 24; i++ ) {
		unDiccionario->insertar_keyTag (unDiccionario, data[i],(void*)i);
	}

	unDiccionario->Do (unDiccionario, (bool(*)(void*,void*,void*)) printAll, NULL);

	for (i = 0; i< 26; i++ ) {
		printf ("%s -> %d\n",data [i],unDiccionario->buscar_keyTag (unDiccionario, data[i]));
	}


	for (i = 0,j = 26; i< 26; i++, j++ ) {
		unDiccionario->insertar_keyTag (unDiccionario, data[i],(void*)j);
	}

	unDiccionario->Do (unDiccionario, (bool(*)(void*,void*,void*)) printAll, NULL);

	for (i = 0; i< 26; i++ ) {
		printf ("%s -> %d\n",data [i],unDiccionario->buscar_keyTag (unDiccionario, data[i]));
	}
	printf ("\nestadisticas: \nEliminados:\t%d \nEntradas:\t%d\nReales:\t\t%d\n",
		unDiccionario->entradasEliminadas, unDiccionario->entradasHechas, unDiccionario->entradasReales);
	
	for (i = 0; i< 10; i++ ) {
		unDiccionario->remover (unDiccionario, data[i]);
	printf ("\nestadisticas: \nEliminados:\t%d \nEntradas:\t%d\nReales:\t\t%d\ntamaño:\t\t%d\n",
		unDiccionario->entradasEliminadas, unDiccionario->entradasHechas, unDiccionario->entradasReales,unDiccionario->tamanioActual);

	}
	for (i = 0; i< 26; i++ ) {
		printf ("%s -> %d\n",data [i],unDiccionario->buscar_keyTag (unDiccionario, data[i]));
	}
	


}

#endif







































