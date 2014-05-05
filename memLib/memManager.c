#include "memManager.h"





void* CALLOC (size_t nmemb, size_t size) {
	void* retorno;
	retorno = calloc (nmemb, size);
	if (retorno == NULL) {
		raise (EMEM);
	}
	return retorno;
}


void* MALLOC (size_t size) {
void* retorno;
	retorno = malloc (size);
	if (retorno == NULL) {
		raise (EMEM);
	}
	return retorno;
}


void FREE (void *ptr) {
	if (ptr == NULL) {
		raise (EMEM);
	}
	free (ptr);
}


void* REALLOC (void *ptr, size_t size) {
void* retorno;

	retorno = realloc (ptr,size);
	if (retorno == NULL) {
		raise (EMEM);
	}
	return retorno;
}












