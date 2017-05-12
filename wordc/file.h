// file.h
#ifndef FILE_H
#define FILE_H

#define MAX 25

#include<string.h>
#include<ctype.h>
#include<stdio.h>

	//FILE* in must be already open before call and have read access
char* input(FILE*);
	//If EOF is reached, NULL will be returned
	//Otherwise, Returns one word all lowercase, no punctuation and last character \0

#endif
