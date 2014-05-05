#include "estado.h"

/** 
 * funciones de soporte: 
 * 		permiten pasar criterios de seleccion a la coleccion en uso.
 * 
 * ESTrem_igualParLexico: compara dos pares lexicos por su atributo "palabra" (para remover)
 * ESTdet_igualParLexico: compara dos pares lexicos por su atributo "palabra" (para detect)
 * ESTcolectaTokens:	  devuelve la palabra de un par lexico. (para collect)
 * ESTdestruyePares:	  libera memoria de un parlexico	(para Do)
 * */

	
bool ESTsel_igualParLexico (void* unpar, void* otro) {
	ParLexico* par1, *par2;
	par1 = (ParLexico*) unpar;
	par2 = (ParLexico*) otro;

	return (par1->unaPalabra->strcmp (par1->unaPalabra, par2->unaPalabra))? TRUE:FALSE;	
}
 
bool ESTrem_igualParLexico (void* self, void* unpar, void* otro){
	ParLexico* par1, *par2;
	par1 = (ParLexico*) unpar;
	par2 = (ParLexico*) otro;
	return ((par1->unaPalabra->strcmp (par1->unaPalabra, par2->unaPalabra))? TRUE:FALSE);	
	
}
/*
bool ESTsel_igualParLexico (void* self, void* unpar, void* otro){
	ParLexico* par1, *par2;
	par1 = (ParLexico*) unpar;
	par2 = (ParLexico*) otro;
	return (par1->unaPalabra->strcmp (par1->unaPalabra, par2->unaPalabra))? TRUE:FALSE;	

		
}*/
void* ESTcolectaTokens (void* tok) {
ParLexico* unPar = (ParLexico*)tok;
	return unPar->unaPalabra;
}

bool ESTdestruyePares (void* self, void* obj, void* clave) {
ParLexico* unPar =(ParLexico* ) obj; 

	unPar->unaPalabra->destruir(unPar->unaPalabra);

	return TRUE;
}

bool printAllVocablo (void* self, void* par, void* clave) {
ParLexico* unPar = (ParLexico*) par;
		printf ("%s \t", unPar->unaPalabra->strval(unPar->unaPalabra));
		return TRUE;
}

/**	fin funciones de soporte. *****************************/



bool ESTcompararCon (struct Estado* self, struct Estado* unEstado) {
	return (self->iEstadoId == unEstado->iEstadoId)? TRUE:FALSE;
}

bool ESTcompararCon_i (struct Estado* self, int estado_id){
	return (self->iEstadoId == estado_id)? TRUE:FALSE;
}


int  ESTgetEstadoId (struct Estado* self) {
	return self->iEstadoId;
}

void ESTagregarVocablo (struct Estado* self, String* token, int iSgte) {
ParLexico* unPar = CALLOC (1, sizeof (ParLexico));

		unPar->unaPalabra = token->strclone(token);
		unPar->iEstadoSgte = iSgte;
		if (self->Vocablos->Mensaje->detect(self->Vocablos, ESTrem_igualParLexico, unPar) == TRUE) {
			unPar->unaPalabra->destruir(unPar->unaPalabra);
			FREE (unPar);
			throw (EESTADOREP_AUT,token);
			return;
		}
		self->Vocablos->Mensaje->add(self->Vocablos,  unPar);
}



void ESTremoverVocablo (struct Estado* self, String* token) {
ParLexico unPar;
Collection* Col;
	     unPar.unaPalabra = token;
	     unPar.iEstadoSgte = -1;
	     Col = self->Vocablos;
	     Col->Mensaje->remove (Col, &unPar, ESTrem_igualParLexico );

}




int ESTgetSgteEstado (struct Estado* self, String* token) {
ParLexico unPar, *rslt;
Collection* col;
	
	unPar.iEstadoSgte = -1;
	unPar.unaPalabra = token;

	if (token != NULL) {
		col = self->Vocablos->Mensaje->select (self->Vocablos,ESTsel_igualParLexico, &unPar);
		rslt = col->Mensaje->getFirst (col);  
		return (rslt == NULL)? -1:rslt->iEstadoSgte;
	}
	return -1; 
}


TipoEstado ESTgetTipo (struct Estado* self) {
	return self->Tipo;
}

void ESTdestruir (struct Estado* self){
	self->Vocablos->Mensaje->Do (self->Vocablos, ESTdestruyePares, NULL);
	self->Vocablos->Mensaje->destruir(self->Vocablos);
	FREE (self);
}

void ESTimprimirEsperados (struct Estado*self) {
	self->Vocablos->Mensaje->Do (self->Vocablos,printAllVocablo, NULL);
	printf ("\n");
}


Estado* ESTnew (int iID, TipoEstado tipo) {
Estado* nuevo;
		nuevo = CALLOC (1, sizeof(Estado));
		nuevo->iEstadoId = iID;
		nuevo->Tipo = tipo;
		nuevo->Vocablos = CollectionClass_New();
		nuevo->compararCon = EstadoClass.compararCon;
		nuevo->compararCon_i = EstadoClass.compararCon_i;
		nuevo->getEstadoId = EstadoClass.getEstadoId;
		nuevo->agregarVocablo = EstadoClass.agregarVocablo;
		nuevo->removerVocablo = EstadoClass.removerVocablo;
		nuevo->getSgteEstado = EstadoClass.getSgteEstado;
		nuevo->getTipo = EstadoClass.getTipo;
		nuevo->destruir = EstadoClass.destruir;
		nuevo->imprimirEsperados = EstadoClass.imprimirEsperados;
		
	return nuevo;
}

void (*_____destruirEstado) (Estado*);

void EstadoClass_GoLive (void) {
		
		EstadoClass.compararCon = ESTcompararCon;
		EstadoClass.compararCon_i = ESTcompararCon_i;
		EstadoClass.getEstadoId = ESTgetEstadoId;
		EstadoClass.agregarVocablo = ESTagregarVocablo;
		EstadoClass.removerVocablo = ESTremoverVocablo;
		EstadoClass.getSgteEstado = ESTgetSgteEstado;
		EstadoClass.getTipo = ESTgetTipo;
		_____destruirEstado =  ESTdestruir;
		EstadoClass.destruir = _____destruirEstado;
		EstadoClass.imprimirEsperados = ESTimprimirEsperados;
		EstadoClass.new = ESTnew;
}


#ifdef ESTADOEXAMPLE
 #include "../framework.c"


void handlerRep (ErrObj* unObj) {
	printf ("Se intento insertar un lexico duplicado! [%s]: Ya existe\n",
		((String*)unObj->extra)->strval(unObj->extra));
	return;
}

int main (void) {
Estado* unEstado;
String* unStr, *unStr2, *unStr3,*unStr4;
sigset_t Set;
Collection* unaCol;
	sigemptyset (&Set);
	sigaddset (&Set, SIGINT);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;
	
	framework_GoLive ();
	initErrMan(SIGINT);
	
	unEstado = EstadoClass.new (1,INICIAL);
	
	printf ("el estado es inicial? %s?\n", unEstado->getTipo (unEstado) == INICIAL?"SI":"NO");
	printf ("el estado es codigo 1? %s\n", unEstado->compararCon_i (unEstado, 1)?"SI":"NO");
	printf ("agregando vocablos..  \n");
	
	/* seteo el handler de estados repetidos .*/
	catch (EESTADOREP_AUT,handlerRep);
	
	unStr = StringClass.new("Hola");
	printf ("insertando %s...\n", unStr->strval (unStr));
	unEstado->agregarVocablo (unEstado, unStr, 2);

	
	unStr2 = StringClass.new("Mundo");
	printf ("insertando %s...\n", unStr2->strval (unStr2));
	unEstado->agregarVocablo (unEstado, unStr2, 3);

	
	unStr3 = StringClass.new("Carajo");
	printf ("insertando %s...\n", unStr3->strval (unStr3));
	unEstado->agregarVocablo (unEstado, unStr3, 4);



	unStr4 = StringClass.new("Carajo");
	printf ("insertando %s...\n", unStr4->strval (unStr4));
	unEstado->agregarVocablo (unEstado, unStr4, 4);
	
	
	catch (EESTADOREP_AUT,NULL);
	/* lo saco. */
	
	unaCol = unEstado->Vocablos->Mensaje->collect(unEstado->Vocablos,(void*(*)(void*))ESTcolectaTokens);

	
	printf ("El siguiente estado con %s es %d\n", unStr->strval(unStr),unEstado->getSgteEstado(unEstado, unStr));
	unStr->destruir(unStr);
	
	printf ("El siguiente estado con %s es %d\n", unStr2->strval(unStr2),unEstado->getSgteEstado(unEstado, unStr2));
	unStr2->destruir(unStr2); 	

	printf ("El siguiente estado con %s es %d\n", unStr4->strval(unStr4),unEstado->getSgteEstado(unEstado, unStr4));
	unStr4->destruir(unStr4);
	
 	printf ("borrando %s del estado... \n",unStr3->strval(unStr3));
 	unEstado->removerVocablo (unEstado, unStr3);
 	
	printf ("El siguiente estado con %s es %d\n", unStr3->strval(unStr3),unEstado->getSgteEstado(unEstado, unStr3));
	unStr->destruir(unStr3);
 	
	return 0;

}


#endif

