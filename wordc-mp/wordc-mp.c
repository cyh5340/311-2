//name: Crystal Ho  section 1
//	Muhammond Hazir Othman section 1
//assignment:2 
#include"wordc.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc, char *argv[]) {
clock_t t=clock();
//define the time start at the beginning of everything

FILE *fp1=fopen(argv[1], "r");
//define the file for:
//one input, two output (one for result and one for time lapse)
int n=0;
n= atoi(argv[4]);//for number of processes
//for storing temp of word
char oneword[100];
char* ac=malloc(4*sizeof(char));

//for getting the size of the file
fseek(fp1, 0, SEEK_END);
int sz = ftell(fp1);
int beginoffset; //for the begin offset of the file	
//for the pipe
int** fd=malloc(n*sizeof(int*));
pid_t childpid;
//for the list 
List* wordList = make_list();

//for the pipe
int i=0;
for (i=0; i< n; i++){
	
	fd[i]=(int*)malloc(2*sizeof(int));
	pipe(fd[i]);

}
//if fail
if((childpid = fork()) == -1){
	perror("fork");
	exit(1);
}
//for chile process
int a=0;
//n-1 because the last process will be left for the parent process

for(a=0; a<n-1; a++){
	childpid=fork();
	if(childpid >=0){ //child process
		beginoffset=sz*a/n;
		//make sure we are not reading half word
		if(beginoffset!=0){
			fseek(fp1, beginoffset-1, SEEK_SET);
			fscanf(fp1, "%c", ac);
			while(isalpha(ac)==true){
				beginoffset++;
				fseek(fp1, beginoffset, SEEK_SET);
				fscanf(fp1, "%c", ac);
			}
		}
		Node s;			
		//then, scan a whole word
		while (beginoffset<sz*(a+1)/n){ 
			fscanf(fp1, "%49[a-zA-Z]", oneword);
			// add
			oneword[0]=tolower(oneword[0]);				
			wordList->add(wordList, oneword);
			//then get the link of that word from list	
			s=search(wordList, oneword);			
			beginoffset+=strlen(s->str);
			s=s->next;
		}	
		close(fd[a][0]);
		write(fd[a][1], s, (sizeof(Node)));
		
//		exit(0);
	}
}

//parent process
//make sure we are not reading half word
	fseek(fp1, sz-(sz/n)-1, SEEK_SET);
	fscanf(fp1, "%c", ac);
	while(isalpha(ac)==true){
		beginoffset++;
		fseek(fp1, beginoffset, SEEK_SET);
		fscanf(fp1, "%c", ac);
	}
	free(ac);
	Node as;		
	//then, scan a whole word
	while (beginoffset<sz*(a+1)/n){ 
		fscanf(fp1, "%49[a-zA-Z]", oneword);
		//make it lower case
		oneword[0]=tolower(oneword[0]);				
		wordList->add(wordList, oneword);
		as= search(wordList, oneword);
		as=as->next;
		beginoffset+=strlen(as->str);
	}
	close(fd[a+1][1]);
	read(fd[a+1][0], as, sizeof(Node));
		

FILE *fp2=fopen(argv[2], "w");
//output for result
wordList->print(wordList, fp2);
fclose(fp2);
free(wordList);

FILE *fp3=fopen(argv[3], "w");
//output for timer
float timer = (float)(clock() - t)/CLOCKS_PER_SEC;
printf("Time Elapsed:\t%fs\n", timer);
fprintf(fp3, "File: %s\nTimer: %f", argv[1], timer);
fclose(fp3);

return 0;
}
