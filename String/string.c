#include "string.h"

int STRstrlen(struct String* self){
	return strlen ((self->unString));
}

int STRstrcmp(struct String* self,struct String* cmp) {

	return (strcmp(self->unString, cmp->unString) == 0);
}

void STRstrcpy(struct String* self,struct String* dataIn) {
	strncpy (self->unString, dataIn->unString , dataIn->iSizeOp);
return;
}

struct String* STRstrclone(struct String* self) {
String* nuevo;
	nuevo = StringClass.newCon ("%s",self->unString);
return nuevo;
}





char* STRstrval(struct String* self) {
	return self->unString;
}


struct String* STRstrcat(struct String* self,struct String* segunda) {
String* nuevo;
	nuevo = StringClass.newCon ("%s%s",self->unString,segunda->unString);
return nuevo;
}

void STRstrcatOnSelf(struct String* self,struct String* segunda) {
String* nuevo;
	nuevo = StringClass.newCon ("%s%s",self->unString,segunda->unString);
	FREE (self->unString);
	memcpy (self,nuevo,sizeof (String));
return;
}


void STRstraddChar (struct String* self, char unChar) {

	if (self->strlen(self) >= self->iSizeOp) {
		self->extender (self, self->iSize + 10);
	}
	
	self->unString [self->strlen(self)] = unChar;
}
void STRextender (struct String* self, int iSize) {
char* nueva;
	nueva = CALLOC (1, iSize);
	strncpy (nueva, self->unString,iSize - 2);
	FREE (self->unString);
	self->unString = nueva;
	self->iSize = iSize;
	self->iSizeOp = iSize - 2 ;

return;
}

void STRdestruir(struct String* self) {
	FREE (self->unString);
	FREE (self);
return;
}

int STR_strstr (struct String* self, String* str) {
	return self->strstr(self,str->strval(str));
}
int STRstrstr (struct String* self, const char* sChr) {
	return (strstr (self->strval(self), sChr) == NULL)? 0:1;
}

int STRstrvec (String* self, const char* sDelimSet, String*** sRsltConj) {
	int iCadenas = 0, i;
	String* selfCpy;
	String* delim;

	char* sStrPoint;
	char* _3erParametro;



	selfCpy = self->strclone (self);
	delim = StringClass.new (sDelimSet);
	
	
		if ((char*)strtok_r (selfCpy->strval(selfCpy), delim->strval(delim),&(_3erParametro)) == NULL) {
			*sRsltConj = NULL;
			return iCadenas;
		}
		/* Cuento la cantidad de tokens en la cadena para reservar memoria. No uso realloc 
		por que busco hacer una funciòn muy estable*/

		for (iCadenas = 1; (char*)strtok_r(NULL, delim->strval(delim),&(_3erParametro)) != NULL; iCadenas++);
		
		/* reservo memoria para el conjunto resultante */
		if ((*sRsltConj = (String**) CALLOC (iCadenas + 1, sizeof (char*)))== NULL) {
			return -1;
		}

		/* regenero las cadenas source por si se perdiò algo en llamadas anteriores */
		selfCpy->strcpy (selfCpy, self);
		delim->destruir (delim);
		delim = StringClass.new (sDelimSet);
		/* creo el conjunto */
		sStrPoint = (char*) strtok_r(selfCpy->strval(selfCpy), delim->strval(delim),&(_3erParametro));

		for (i = 0; i < iCadenas; i++){
			*((*sRsltConj) + i) = StringClass.new (sStrPoint);
			sStrPoint = (char*)strtok_r(NULL, delim->strval(delim),&(_3erParametro));
		}
		
		/* retorno la cantidad de cadenas en conjunto :) (el -1 es por que 
			cuenta el nulo, y no me interesa)*/
	return iCadenas;
}

struct String* STRnew (const char* cadena) {
String* nuevo;

	nuevo = CALLOC (1, sizeof(String) + 2);
	nuevo->unString = CALLOC (1, strlen(cadena) + 2);

	strcpy(nuevo->unString,cadena);

	nuevo->iSize = strlen(cadena) + 2;
	nuevo->iSizeOp = strlen(cadena);
	nuevo->strlen = StringClass.strlen;
	nuevo->strcmp = StringClass.strcmp;
	nuevo->strvec = StringClass.strvec;
	nuevo->strcpy = StringClass.strcpy;
	nuevo->strclone = StringClass.strclone;
	nuevo->strval = StringClass.strval;
	nuevo->strcat = StringClass.strcat;
	nuevo->strcatOnSelf = StringClass.strcatOnSelf;
	nuevo->straddChar = StringClass.straddChar;
	nuevo->extender = StringClass.extender;
	nuevo->destruir = StringClass.destruir;
	nuevo->_strstr =  StringClass._strstr;
	nuevo->strstr =  StringClass.strstr;

return nuevo;
}


struct String* STRnewCon (const char* formato, ...) {
va_list lista;
char unString [500 + 2];
String* nuevo;

	va_start (lista, formato);
	vsnprintf(unString, 500, formato, lista);
	va_end (lista);
	nuevo = StringClass.new (unString);

return nuevo;
}

void (*________destruirString) (String*);

void StringClass_GoLive (void) {
	
	StringClass.strlen = STRstrlen;
	StringClass.strcmp = STRstrcmp;
	StringClass.strcpy = STRstrcpy;
	StringClass.strclone = STRstrclone;
	StringClass.strval = STRstrval;
	StringClass.strcat = STRstrcat;
	StringClass.strcatOnSelf = STRstrcatOnSelf;
	StringClass.straddChar = STRstraddChar;
	StringClass.extender = STRextender;
	________destruirString = STRdestruir;
	StringClass.destruir = ________destruirString;
	StringClass.new = STRnew;
	StringClass.newCon = STRnewCon;
	StringClass.strvec = STRstrvec;
	StringClass._strstr = STR_strstr;
	StringClass.strstr = STRstrstr;

}



/*#define STRINGEXAMPLE*/
#ifdef STRINGEXAMPLE
#include <stdio.h>
int main (void) {
String* unString;
String** rslt;
String* unStr;
int  i;
const char* pepe = "| hola1 | hola 2 | hola3 | hola 4 | hola5 | hola 6 | hola7 | hola 8 | hola9 | hola 10 | hola11 | hola | hola |";

	StringClass_GoLive();


	unString = StringClass.new ("");

	for (i = 0; i< strlen(pepe); i++) {
		unString->straddChar(unString, pepe[i]);

	}
	printf ("\n********\n%s\n*********\n", unString->strval(unString));
	i = unString->strvec (unString,"| ",&rslt);
	printf ("hola cola! %d\n", i);
	i--;
	for (;i>=0;i--) {
		printf ("%s\n",rslt[i]->strval (rslt[i]));
	}
	unString->destruir (unString);

}













#endif
