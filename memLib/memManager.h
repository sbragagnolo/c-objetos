#ifndef MEMMANAGERHEADER
#define MEMMANAGERHEADER

#include <malloc.h> 
#include <signal.h> 

#define EMEM SIGRTMAX



void* CALLOC 	(size_t nmemb, size_t size);
void* MALLOC 	(size_t size);
void* REALLOC 	(void *ptr, size_t size);
void  FREE 	(void *ptr);


#endif









