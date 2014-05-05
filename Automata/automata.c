#include "automata.h"
/**
 * Error EESTINIINX: error fatal! a quien lo capture, termine la ejecucion.
 * PARA USO DE AUTOMATA VER LINEA 263. MUY IMPORTANTE.
 **/


/**
 *	Funciones auxiliares para manejo de Colecciones. 
 * 
 * 		AUTselEstado (); Ayuda a obtener una coleccion con un estado buscado. 
 * 		AUTdetEstado (); Ayuda a indicar si un estado ya existe. 
 *  
 **/

	bool AUTselEstado (void* unEstado, void* otroEstado) {
		Estado *E1, *E2;
		E1 = (Estado*) unEstado;
		E2 = (Estado*) otroEstado;
		
		if (E1->compararCon(E1,E2) == TRUE) return TRUE;
		return FALSE;
	}

	bool AUTdetEstado (void* col, void* unEstado, void* otroEstado) {
		return AUTselEstado (unEstado, otroEstado);
	}
	
	bool ESTdestruyeEstados (void* self, void* obj, void* clave) {
		Estado* E = (Estado*) obj;
		E->destruir(E);
		return TRUE;	
	}
	

	
/** Fin soporte a Colecciones 	*********/


/**
 *	Funciones de soporte a Diccionario. 
 * 		Con el fin de introducir un numero como tag, tengo que "sobreescribir" 
 * 		el método de hasheo.  
 **/
 char* EstadoSgtehashCode (hasheable* hash) {
 	char strCode [CODELONG + 1];
	char * sHashCode;
	int sgte = (int) hash->unObjeto;
	
		snprintf (strCode, CODELONG, "%d", sgte);
		sHashCode = CALLOC (strlen(strCode),sizeof(char));
		strcpy (sHashCode,strCode);

	return sHashCode;
 }
 
 
 
 /**
  * funciones de soporte de tratamiento de caracteres. Avanzado, solo para parseo, no tiene sentido agregarlo
  * en el obj String, solo significa una sobrecarga de memoria. 
  * */

void to_lower (String* linea) {
char* line;
int iLong, i;
	iLong = linea->strlen (linea);
	line = linea->strval(linea);
	for (i = 0; i < iLong; i++) {
		line [i] = tolower (line[i]);
	}
}

void limpiarLinea (String* linea, String* terminador){
char* line;
char* ptr;
	
	line = linea->strval (linea);
	if (strncmp (line, COMMENT, COMLONG) == 0) strcpy (line, "");
 
	ptr = strstr (line, terminador->strval(terminador));

	if (ptr != NULL) {
		strcpy (ptr, "");
	}

}

void liberarArgs (int argc, String ** argv ) {
int i;
	for (i = 0; i < argc ; i++ ) FREE (argv [i]);

}

/*****************************************/
/**
 *	Agrega un estado nuevo al automata.  
 **/
 
 
void AUTagregarEstado (Automata* self, int iCod, TipoEstado tipo) {
Estado* nuevoEstado;
	nuevoEstado = EstadoClass.new (iCod,tipo);
	if (self->Estados->Mensaje->detect (self->Estados,AUTdetEstado,nuevoEstado) == TRUE){
			throw (EESTADOREP_AUT, 
					StringClass.newCon ("Se intento ingresar el estado %d: el estado ya existe",
					iCod)); 
			return;
	}
	self->Estados->Mensaje->add(self->Estados, nuevoEstado);
return;
}

void AUTagregarVocabloEn (Automata* self,int iCod, String* token, int iSgte){
Estado* unEstado;
Collection* col;

	/** creo un estado "fake" para poner el codigo parametro con fines de busqueda*/
	unEstado = EstadoClass.new(iCod, NORMAL);
	
	/* manejo la coleccion "Estados" desde col para evitar largo excesivo */
	
	col = self->Estados;
	col = col->Mensaje->select(col, AUTselEstado, unEstado);
	
	/** busco el estado en el que debo agregar el vocablo */
	
	unEstado->destruir (unEstado);
	
	/** borro el estado "fake" y uso la variable para meter el estado que busco. */
	
	unEstado = col->Mensaje->getFirst (col);
	
	  /** si no esta es por que no esta creado .*/
	if (unEstado == NULL) {
			throw (EESTADOINVALIDO, 
				StringClass.newCon("Se quizo agregar el vocablo [%s] en el estado [%d]: dicho estado no existe",
									token->strval(token),
									iCod));
			return;
	}
	 /** agrego el vocablo en el estado  8-) todos felices .*/
	unEstado->agregarVocablo(unEstado, token, iSgte);
	
	return;
}

/**	inserto un clone, recordar liberar memoria origen si solo esta para esto. */
void AUTsetDelimSet (Automata* self, String* delim) {
	self->sDelimSet = delim->strclone (delim);
return;
}
void AUTsetSepSet (Automata* self, String* sep) {
	self->sSepSet = sep->strclone (sep);
return;
}

/** destruye todo, los estados las colecciones strings etc. */
void AUTdestruir (Automata* self) {

	self->Estados->Mensaje->Do (self->Estados, ESTdestruyeEstados, NULL);
	self->Estados->Mensaje->destruir (self->Estados);
	self->fnDir->destruir(self->fnDir);
	self->sDelimSet->destruir (self->sDelimSet);
	FREE (self);
}

/** agrega una funcion al directorio. la misma se va a poder acceder 
 * luego por medio  del codigo dado como parametro. */
 
 
void AUTagregar_fn (Automata* self, String*(*fn)(void*,int,String**), int codigo, void* fstParam) {
hasheable hash;
OHF* fnHash;
	
	/** EstadoSgtehashCode: funcion definida al principio. */
	hash.hashString = EstadoSgtehashCode;
	hash.unObjeto = (void*)codigo;
	
	fnHash = CALLOC (1, sizeof(OHF));
	
	fnHash->fnComando = fn;
	fnHash->fstParam = fstParam;
	if (self->fnDir->insertar_Hasheable (self->fnDir, &hash, fnHash) == HASH_FAIL) {
			throw (EINSDICT, 
					StringClass.newCon ("Error al insertar las funcion de codigo %d",
					codigo));
	}
return;
} 

Estado* AUTgetOne (struct Automata* self,int iCod) {
Estado* EstFake, * Est;
	EstFake = EstadoClass.new(iCod,NORMAL);
	Est = self->Estados->Mensaje->getOne (self->Estados, AUTselEstado, EstFake);
	EstFake->destruir (EstFake);
	if (Est == NULL) {
		/** ERROR FATAL!! A QUIEN LO CAPTURE: TERMINAR EJECUCION! */
		throw (EESTINIINX, 
		StringClass.newCon ("Error en automata: no se pudo resolver el estado inicial, asegurese de que el mismo es estado 1"));
	}
	return Est;
}



atomoExe* AUTparsearString (Automata* self, String* toParse) {
String** tokLine;
Estado* estado;
atomoExe* Atomo;
int tokCant;
int i = 0;
int iSgte;


	/** corto la entrada a parsear en base a los delimitadores de token del automata*/
	tokCant = toParse->strvec (toParse, self->sDelimSet->strval(self->sDelimSet), &tokLine);

	/* obtendo el primer estado del automata. Este estado es obligatorio! y unico inicial!*/
	estado = self->getOne(self, 1);
	
	/** 
	 * 	mientras no este en un estado final, pido el siguiente estado en base a un token (ver estado.c / .h) 
	 *	si siguiente es -1 es por que hay un error sintactico.
	 *  sino, obtengo el siguiente estado (estado dado por el numero iSgte) 
	 **/
	
	while (estado->getTipo(estado) != FINAL && i < tokCant) {

		iSgte = estado->getSgteEstado (estado, tokLine [i]);
		if (iSgte == -1) {
			throw (ESTXAUT, estado);
			return NULL;
		}
		estado = self->getOne(self, iSgte);
		tokLine [i]->destruir (tokLine [i]);
		i++;
	}
	/**
	 *  si el ultimo estado obtenido no es final es por que se acabaron los tokens antes de 
	 *  llegar al estado final, esto es un error de sintaxis. 
	 **/
	if (estado->getTipo (estado) != FINAL) {
			throw (ESTXAUT, estado);
			return NULL;	
	}
	/** obtengo el siguiente estado, que en un estado final es en verdad un codigo de operacion .*/
	iSgte = estado->getSgteEstado (estado, tokLine [i]);

    /** si siguiente es -1 es por que el estado final al que llegamos no admite el ultimo token dado */
	if (iSgte == -1) {
		throw (ESTXAUT, estado);
		return NULL;
	}
    /** reservo memoria para hacer un atomo de ejecucion. NOTA: TODO analizar la necesidad de un obj Atomo,
     *  se van a manejar muchos atomos de ejecución, pero el unico generador es este metodo, en este momento 
     *  lo supongo una carga innecesaria de memoria. 
     **/
	Atomo = CALLOC (1, sizeof(atomoExe));
	
	 /** 
	  * Obtengo ek OnHashFn de la tabla de hash. 
	  **/
	  
	Atomo->Com = self->getCommand (self, iSgte);
	
	/* paso al siguiente token, es para anotar lo que va a usar el atomo como argumentos. */
	i++;
	/** si no obtengo una funcion contreta es por que la misma no fue implementada */
	if (Atomo->Com == NULL || Atomo->Com->fnComando == NULL) {
			throw (ECOMNOIMP, 
			StringClass.newCon ("Error de parseo: el comando %d no ha sido implementado!!", iSgte)); 
	}
	/** paso los argumentos .*/
	Atomo->argv = &(tokLine [i]);/** SUMAMENTE IMPORTANTE, RECORDAR LIBERAR LA MEMORIA DE LOS ARGUMENTOS EN */
	Atomo->argc = tokCant -i;    /** ALGUN PUNTO LUEGO DE LA EJECUCION. */
	 
return Atomo;
}

OHF* AUTgetCommand(struct Automata* self, int fn_id){
	
	hasheable Hash;
	Hash.unObjeto = (void*) fn_id;
	Hash.hashString = EstadoSgtehashCode;
return	self->fnDir->buscar_Hasheable (self->fnDir, &Hash);
}

atomoExe* AUTparsearLinea (Automata* self, char* toParse) {
String* str; 
atomoExe* atom;
	str = StringClass.new (toParse);
	atom = self->parsearString (self,str);
	str->destruir(str);
return atom;
}



Collection* AUTparsearArchivo (Automata* self,char* path) {
Collection* plan;
FileObj* arc;
char fileLine [400 + 2];
String* linea, *buffer, *nula;
atomoExe* atomo;
int i = 0;
	
	arc = FileObjClass.newCon (path, "r");
	if (arc == NULL) return NULL;

	plan = CollectionClass_New();

	nula = StringClass.new ("");

	do {
		buffer = StringClass.new ("");

		while (arc->leerLinea (arc, fileLine, 400)) {
			i++;

			linea = StringClass.new(fileLine);
			buffer->strcatOnSelf(buffer,linea);
			linea->destruir(linea);

			if (buffer->_strstr(buffer,self->sSepSet)){break;}			
		}
		limpiarLinea (buffer, self->sSepSet);
		if (arc->error (arc)) {
				printf ("error al leer %s\n",path); 
				break;
		}  

		if (arc->eof(arc)) break;
		if (!buffer->strcmp(buffer, nula)){
			if ((atomo = self->parsearString (self,buffer)) != NULL) {
					plan->Mensaje->add(plan, atomo);
			}
			else {		
					printf ("[%s]<-- error en la linea [%d]\n",buffer->strval(buffer), i); 
					break;
			}
		}
		
	} while (TRUE);
	
return plan;
}




Automata* AUTnew (void) {
	Automata* aut;
	aut = CALLOC (1, sizeof (Automata));
	aut->agregar_fn = AutomataClass.agregar_fn;
	aut->agregarEstado = AutomataClass.agregarEstado;
	aut->agregarVocabloEn = AutomataClass.agregarVocabloEn;
	aut->getCommand = AutomataClass.getCommand;
	aut->getOne = AutomataClass.getOne;
	aut->parsearArchivo = AutomataClass.parsearArchivo;
	aut->parsearLinea = AutomataClass.parsearLinea;
	aut->parsearString = AutomataClass.parsearString;
	aut->setDelimSet = AutomataClass.setDelimSet;
	aut->setSepSet = AutomataClass.setSepSet;
	aut->destruir = AutomataClass.destruir;
	aut->Estados = CollectionClass_New();
	aut->fnDir = DictionaryClass.new (&DictionaryClass, 20);
return aut;
}



/**
 * 	Arma un set de caracteres para el automata
 * 	Acepta los comodines:
 *  
 * 		blanco, coma, pp, pipe
 * 
 *	Debido a que los usa el automata de automatas 
 *  para interpretar la configuracion 
 * */

String* armaSet (int argc, String** delims) {
int i;
/* espacio para set nuevo*/
String* sDelimSet;

/* comodines sintacticos */
String* comodinBLANCO;
String* comodinPIPE;
String* comodinPP;
String* comodinCOMA;
String* comodinENTER;

/* valores */
String* BLANCO;
String* PIPE;
String* PP;
String* COMA;
String* ENTER;

/* inicializacion */
	sDelimSet = StringClass.new ("");
	
	BLANCO = StringClass.new (BLANKVAL);
	PIPE = StringClass.new (PIPEVAL);
	PP = StringClass.new (PPVAL);
	COMA = StringClass.new (COMAVAL);
	ENTER = StringClass.new (ENTERVAL);
	
	comodinBLANCO = StringClass.new (BLANKCOM);
	comodinPIPE = StringClass.new (PIPECOM);
	comodinPP = StringClass.new (PPCOM);
	comodinCOMA = StringClass.new (COMACOM);
	comodinENTER = StringClass.new (ENTERCOM);
	
/* armado */
	for (i = 0 ; i < argc ; i++ ){
		if (delims[i]->strcmp (delims[i],comodinBLANCO)){
			sDelimSet->strcatOnSelf (sDelimSet,BLANCO);
		}
		else if (delims[i]->strcmp (delims[i],comodinPIPE)){
			sDelimSet->strcatOnSelf (sDelimSet,PIPE);
		}
		else if (delims[i]->strcmp (delims[i],comodinPP)){
			sDelimSet->strcatOnSelf (sDelimSet,PP);
		}
		else if (delims[i]->strcmp (delims[i],comodinCOMA)){
			sDelimSet->strcatOnSelf (sDelimSet,COMA);
		}
		else if (delims[i]->strcmp (delims[i],comodinENTER)){
			sDelimSet->strcatOnSelf (sDelimSet,ENTER);
		}
		else {		
			sDelimSet->strcatOnSelf (sDelimSet,delims[i]);
		}
	}
return sDelimSet;
}

/**
 *	Arma un String de delimitacion por medio de armaSet, 
 *  y lo setea a un automata dado. 
 * */
 
 
String* cargaDelim (void* sel, int argc, String** delims) {
	String* delim;
	Automata* self = (Automata*) sel;
 		delim = armaSet (argc,delims);
 		self->setDelimSet(self,delim);
 	return NULL;
}

/**
 *	Arma un String de separacion por medio de armaSet, 
 *  y lo setea a un automata dado. 
 * */

String* cargaSep (void* sel, int argc, String** delims) {
	String* seps;
	Automata* self = (Automata*) sel;
 		seps = armaSet (argc,delims);
 		self->setSepSet (self,seps);
 	return NULL;
}

/**
 *	Crea un estado nuevo , 
 *  y lo setea en un automata dado. 
 * */
int to_number (String* unNum){
	char* line;
	char* err;
	int num = 0;
	
	line = unNum->strval (unNum);
	num = strtol(line, &err, 10);
	
	if (num < 1) return -1;
	
	if (err != NULL && strcmp (err, "") ) return -1;
	
	return num;
}


/**
 *	transforma a "TipoEstado" segun constantes lexicas. 
 * 	final, normal, inicial.  
 **/
 
 
TipoEstado to_type (String* unTipo) {
TipoEstado tipo = -1;
String* comFINAL;
String* comNORMAL;
String* comNORMAL2;
String* comINICIAL;

	comFINAL = StringClass.new (FINALCOM);
	comINICIAL = StringClass.new (INICIALCOM);
	comNORMAL = StringClass.new (NORMALCOM);
	comNORMAL2 = StringClass.new ("");
	
	tipo = (unTipo->strcmp(unTipo, comFINAL))? FINAL:tipo;
	tipo = (unTipo->strcmp(unTipo, comINICIAL))? INICIAL:tipo;
	tipo = (unTipo->strcmp(unTipo, comNORMAL))? NORMAL:tipo;
	tipo = (unTipo->strcmp(unTipo, comNORMAL2))? NORMAL:tipo;
	
	 comFINAL->destruir(comFINAL);
	 comINICIAL->destruir(comINICIAL);
	 comNORMAL->destruir(comNORMAL);
	 comNORMAL2->destruir(comNORMAL2);
	 	  
return tipo;
}
/**
 *	Crea un estado dentro del automata dado.  
 **/
String* creaEstado (void* sel, int argc, String** argv) {
int iEstado;
TipoEstado tipo;
Automata* self = (Automata*) sel;

	if (argc < 2) {
		 throw (EBADPARAM_AUT, StringClass.newCon (
		 		"[En Carga x Archivo]\n\tError al crear estado numero %d: Parametros insuficientes",iEstado));
 		 liberarArgs (argc,argv);
			return NULL;
	}
	iEstado = to_number (argv[0]);
	
	tipo = to_type (argv[1]);
	
	liberarArgs (argc,argv);

	if (iEstado == -1 || tipo == -1) {
		 throw (EBADPARAM_AUT, StringClass.newCon ("[En Carga x Archivo]\n\tError al crear estado numero %d del tipo %s",iEstado, 
		 					(tipo == FINAL)?"FINAL":
		 					(tipo == INICIAL)? "INICIAL":
		 					(tipo == NORMAL)? "NORMAL":
		 							"DESCONOCIDO"));

		return NULL;
	}
	self->agregarEstado (self,iEstado, tipo);
	
return NULL;
}
/**
 *	agrega un vocablo dentro de un estado de un automata 
 **/
String* agregaEn (void* sel, int argc, String** argv){
	int iEstado;
	int iSgte;
	String* token;
	Automata* self = (Automata*) sel;
	int i;
	if (argc < 3 || argc % 2 == 0){
		 throw (EBADPARAM_AUT, StringClass.newCon ("[En Carga x Archivo]\n\tError al agregar vocablo en estado numero %d: parametros insuficientes",iEstado));
		 liberarArgs (argc,argv);
			return NULL;	
	}

	iEstado = to_number (argv [0]);
	
	
	if (iEstado == -1) {

		 throw (EBADPARAM_AUT, StringClass.newCon ("[En Carga x Archivo]\n\tError al agregar vocablo en estado numero %d",iEstado));
		 liberarArgs (argc,argv);
			return NULL;
	}
	
	 
	for (i = 1; i < argc; i++) {
		token = argv [i];
		i++;
		
		if ((iSgte = to_number (argv [i])) == -1) {
			printf ("%s \n",token->unString);
			throw (EBADPARAM_AUT, StringClass.newCon (
									"[En Carga x Archivo]\n\tError al agregar vocablo en el estado:%d \
									\n\tToken => [%s]\n\tEstado Siguiente => [%d]\n",iEstado,token->strval(token),iSgte));
			liberarArgs (argc,argv);
			return NULL;
		}

		self->agregarVocabloEn(self, iEstado, token, iSgte);
	}

liberarArgs (argc,argv);
return NULL;
}

/**
 * carga al automata padre para poder crear automatas en base a archivos. 
 * */
int cargarConfiguracion (Automata* auto1) {
	String* strTemp;
	
	
	strTemp = StringClass.new (" ,|:");
	auto1->setDelimSet (auto1, strTemp);
	strTemp->destruir(strTemp);
	
	strTemp = StringClass.new ("\n");
	auto1->setSepSet (auto1, strTemp);
	strTemp->destruir(strTemp);
	
	
	/** creo estados de automata */
	auto1->agregarEstado (auto1, 1, INICIAL);
	auto1->agregarEstado (auto1, 2, FINAL);
	auto1->agregarEstado (auto1, 3, FINAL);
	auto1->agregarEstado (auto1, 4, NORMAL);
	auto1->agregarEstado (auto1, 5, FINAL);

	/** creo funciones */

	auto1->agregar_fn (auto1,cargaDelim, 1, NULL);
	auto1->agregar_fn (auto1,cargaSep, 2, NULL);
	auto1->agregar_fn (auto1,creaEstado, 3, NULL);
	auto1->agregar_fn (auto1,agregaEn, 4, NULL);
	
	
	/** definicion de los vocablos de estado 1*/
	strTemp = StringClass.new ("set");
	auto1->agregarVocabloEn (auto1, 1, strTemp, 2);
	strTemp->destruir(strTemp);
	strTemp = StringClass.new ("crear");
	auto1->agregarVocabloEn (auto1, 1, strTemp, 3);
	strTemp->destruir(strTemp);
	strTemp = StringClass.new ("agregar");
	auto1->agregarVocabloEn (auto1, 1, strTemp, 4);
	strTemp->destruir(strTemp);
	
	/** definicion de los vocablos de estado 2*/
	strTemp = StringClass.new ("delimitador");
	auto1->agregarVocabloEn (auto1, 2, strTemp, 1); /** definir fn*/
	strTemp->destruir(strTemp);
	strTemp = StringClass.new ("separador");
	auto1->agregarVocabloEn (auto1, 2, strTemp, 2); /** definir fn*/
	strTemp->destruir(strTemp);
	
	/** definicion de los vocablos de estado 3*/
	strTemp = StringClass.new ("estado");
	auto1->agregarVocabloEn (auto1, 3, strTemp, 3); /** definir fn*/
	strTemp->destruir(strTemp);
	
	/** definicion de los vocablos de estado 4*/
	strTemp = StringClass.new ("en");
	auto1->agregarVocabloEn (auto1, 4, strTemp, 5); 
	strTemp->destruir(strTemp);

	/** definicion de los vocablos de estado 5*/
	strTemp = StringClass.new ("estado");
	auto1->agregarVocabloEn (auto1, 5, strTemp, 4); /** definir fn*/ 
	strTemp->destruir(strTemp);
	
	return 0;
}
/** aplica un nodo de un plan a un objetivo */

bool aplicador (void* col, void* atom, void* aut) {
atomoExe* atomo = (atomoExe*) atom;

	atomo->Com->fnComando ((atomo->Com->fstParam == NULL)? aut:atomo->Com->fstParam,atomo->argc, atomo->argv);
	return TRUE;
}

Automata* AUTnewCon (char* path) {
Automata* auto1, *nuevo;
Collection* plan;

	auto1 = AutomataClass.new();
	nuevo = AutomataClass.new();
	cargarConfiguracion (auto1);
	
	/** 
	 *  set delimitador BLANCO PP COMA PIPE
	 * 	set separador ;
	 *  crear estado 1 
	 *  agregar en estado 1: hola|1, mundo|2, pedro|3, pepe|1
	 * */
	plan = auto1->parsearArchivo (auto1, path);
	plan->Mensaje->Do(plan, aplicador, nuevo);
	auto1->destruir (auto1);
	return nuevo;
	
}


void (*____destruirAutomata) (Automata*);

void Automata_GoLive (void) {
	FileObjClass_GoLive ();
	EstadoClass_GoLive ();
	AutomataClass.agregar_fn = AUTagregar_fn;
	AutomataClass.agregarEstado = AUTagregarEstado;
	
	AutomataClass.agregarVocabloEn = AUTagregarVocabloEn;
	AutomataClass.getCommand = AUTgetCommand;
	AutomataClass.getOne = AUTgetOne;
	AutomataClass.parsearArchivo = AUTparsearArchivo;
	AutomataClass.parsearLinea = AUTparsearLinea;
	AutomataClass.parsearString = AUTparsearString;
	AutomataClass.setDelimSet = AUTsetDelimSet;
	____destruirAutomata = AUTdestruir; 
	 AutomataClass.destruir = ____destruirAutomata;
	 AutomataClass.new = AUTnew;
	 AutomataClass.newCon = AUTnewCon;
	 AutomataClass.setSepSet = AUTsetSepSet;
}


/*#define AUTEXAMPLE*/
#ifdef AUTEXAMPLE
#include "../framework.c"

bool fn2 (void* self, void* n, void* clave) {
atomoExe* nodo = (atomoExe*) n; 
	nodo->Com->fnComando (nodo->Com->fstParam,nodo->argc, nodo->argv);
	return TRUE;
}


String* fn1 (void* key, int cant, String** argv) {
	int i;
	String* fst = (String*) key;
	
	printf ("fst:[%s]\n",fst->strval (fst));
	for (i = 0; i < cant ; i++ ){ 
		printf ("%s\t", argv [i]->strval (argv [i]));
	}
	printf ("\n");
	return NULL;
} 
bool aplica (void* col, void* ato, void* clave) {
atomoExe* atom;
	atom = (atomoExe*) ato;
	atom->Com->fnComando(atom->Com->fstParam, atom->argc, atom->argv);
	return TRUE; 	
}


 
void insDicHandler (ErrObj* unErr) {
 	String* str = (String* )unErr->extra;
 	printf ("Error  %s\n", str->strval (str));
 	exit (EBADPARAM_AUT);
}

void autbadparam (ErrObj* unErr) {
 	String* str = (String* )unErr->extra;
 	printf ("Error  %s\n", str->strval (str));
 	
 	exit (EBADPARAM_AUT);
}


 void idlehandler (ErrObj* unErr) {
 }
 void stxHandler (ErrObj* unErr) {
 	Estado* est;
 	printf ("Error de sintaxis, se esperaba:\n");
 	est = (Estado* )unErr->extra;
 	est->imprimirEsperados(est);
 }
 int main (int argc, char* argv [] ) {
 	Automata* a;
 	Estado* estado;
 	int i,j;
 	Collection* col;
 	String* str;
 	sigset_t Set;
	

	
	sigemptyset (&Set);
	sigaddset (&Set, SIGINT);
	if (sigprocmask (SIG_BLOCK, &Set, NULL)) return -1;
 	
 	framework_GoLive ();
 	initErrMan(SIGINT);
 	Automata_GoLive();
 	
 	catch (EEOF_FILE,idlehandler);
 	catch (ESTXAUT,stxHandler);
 	catch (EBADPARAM_AUT,autbadparam);
 	
/** 
 * ejemplo de automata con una carga realmente grande. 100 estados y 200 vocablos por estado. 
 * Para el tp es mas que suficiente 
 **/
 	a = AutomataClass.new();
 	for (i = 0; i < 100; i++) {
 		a->agregarEstado (a, i, (rand () % 10 < 6)? INICIAL:FINAL);
 		for (j = 0; j < 200;j++) {
 			str = StringClass.newCon ("vocablo%d",j);
 			a->agregarVocabloEn (a, i, str, i + 1);
 			str->destruir (str);
 		}
 	}
 
 	printf ("Configuración del automata (estados) \n");
 	a->setDelimSet (a, StringClass.new (";: |"));
 	a->setSepSet (a, StringClass.new (";: |"));
 	for (i = 0; i < 100; i++) {
 		printf ("Estado :%d Vocablos: \n",i);
		estado = a->getOne(a, 1);
		estado->imprimirEsperados(estado);
 		
 	}
 	
 	a->destruir(a);

 	a = AutomataClass.newCon("auto.conf");
 	
	a->agregar_fn (a, fn1, 1, StringClass.new ("Holaaaa! PARAM!!!"));
	
	
	printf ("Automata configurado como:\n");
	printf ("\tlimitadores %s\n", a->sDelimSet->unString);

	printf ("\tseparadores %s\n", a->sSepSet->unString);
	printf ("estado: %d \t Vocablos\n\t",1);
	estado = a->getOne(a, 1);
	estado->imprimirEsperados(estado);
	
	printf ("estado: %d \t Vocablos\n\t",2);
	estado = a->getOne(a, 2);
	estado->imprimirEsperados(estado);
	printf ("estado: %d \t Vocablos\n\t",3);
	estado = a->getOne(a, 3);
	estado->imprimirEsperados(estado);
	printf ("******************************* \n");
	
	printf ("ejemplo de parseo: [hola mundo!!! ]\n");
	a->parsearLinea (a, "hola mundo!!! \n");
	printf ("******************************* \n");
	printf ("\n ejemplo de parseo archivo top.prs\n");
	col = a->parsearArchivo (a, "top.prs");
	col->Mensaje->Do(col, aplica, NULL);
	printf ("******************************* \n");
 return 0;
 }
 
 
 
 
 #endif
