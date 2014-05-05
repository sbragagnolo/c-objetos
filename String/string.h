#ifndef STRINGHEADER
#define STRINGHEADER
#include <string.h>
#include <stdarg.h>
#include "../memLib/memManager.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct String {
	char* unString;
	int iSize;	/**tamaño real*/
	int iSizeOp;	/** tamaño operativo*/
	int (*strlen)(struct String*);
	int (*strcmp)(struct String* self,struct String* cmp);
	void (*strcpy)(struct String* self,struct String* dataIn);
	struct String* (*strclone)(struct String* self);
	int (*strvec)(struct String* self, const char* delimSet, struct String***Rslt);
	char* (*strval)(struct String*);
	struct String* (*strcat)(struct String* self,struct String* segunda);
	void (*strcatOnSelf)(struct String* self,struct String* segunda);
	void (*straddChar) (struct String* self, char);
	int (*_strstr) (struct String* self, struct String*);
	int (*strstr) (struct String* self, const char*);
	void (*extender) (struct String* self, int);
	void (*destruir)(struct String* self);
}String;


typedef struct ____StringClass {
	int (*strlen)(struct String*);
	int (*strcmp)(struct String* self,struct String* cmp);
	void(*strcpy)(struct String* self,struct String* dataIn);
	struct String* (*strclone)(struct String* self);
	int (*strvec)(struct String* self, const char* delimSet, struct String***Rslt);
	char* (*strval)(struct String*);
	struct String* (*strcat)(struct String* self,struct String* segunda);
	void (*strcatOnSelf)(struct String* self,struct String* segunda);
	void (*straddChar) (struct String* self, char);
	int (*_strstr) (struct String* self, String*);
	int (*strstr) (struct String* self, const char*);
	void (*extender) (struct String* self, int);
	void (*destruir)(struct String* self);
	struct String* (*new) (const char*);
	struct String* (*newCon) (const char* formato, ...);
}____StringClass;

____StringClass StringClass;
void StringClass_GoLive (void);
#endif

