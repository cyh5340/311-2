/*
  Developers:
	John Barczynski (Section __)
	Matthew Heilman (Section 01)
  
  Class:
  	CMPSC 311
  	
  Assignment:
  	Homework 1  
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"iterator.h"
#include"file.h"

//*		// Remove one of the '/' for secondary structure
int main(int argc, char* argv[]){
//	=======================================================
/*/
int main(){
	int argc = 4;
	char **argv = malloc(sizeof(char) * 4 * 15);
//	argv[1] = "pangur ban";
//	argv[1] = "hamlet";
	argv[1] = "Arabian nights";
	argv[2] = "countfile";
	argv[3] =  "runtime";
//*/
	
//	=======================================================
// 	SETUP
//	=======================================================
	if(argc < 4)
	{
		printf("All arguments not specified");
		return 1;
	}
	
	
//	=======================================================
// 	PHASE 1: Read from file and add to linked list
//	=======================================================
	clock_t t = clock();

	FILE *passage = fopen(argv[1], "r");
	char* temp;
	List* wordList = make_list();
	Node previousNode;
	while((temp = input(passage)) != NULL && (previousNode = wordList->add(wordList, temp)) != NULL){
		free(temp);
	}
	fclose(passage);
	
	
//	=======================================================
//	PHASE 2: Write linked list to file
//	=======================================================
	FILE* outfile = fopen(argv[2], "w");
	wordList->print(wordList, outfile);
	fclose(outfile);
	free(wordList);
	
	
//	=======================================================
//	PHASE 3: Write timer to file
//	=======================================================	

	float timer = (float)(clock() - t)/CLOCKS_PER_SEC;
	FILE* outTime = fopen(argv[3], "w");
	printf("Time Elapsed:\t%fs\n", timer);
	fprintf(outTime, "File: %s\nTimer: %f", argv[1], timer);
	fclose(outTime);
	return 0;
}


