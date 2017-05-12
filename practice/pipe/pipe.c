#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(int argv, char* argc[]){

int** fd=malloc(10*sizeof(int*));
pid_t childpid;
char readbuffer[80];

int a[]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int b=0;
int i=0;

for (int i=0; i< 10; i++){
//fork();
fd[i]=(int *)malloc(2*sizeof(int));

pipe(fd[i]);
if((childpid = fork()) == -1){
	perror("fork");
	exit(1);
}

if(childpid ==0){ //child process
	printf("in if: %d\n", a[i]);
	close(fd[i][0]);
	write(fd[i][1], a[i], (sizeof(int)));
	exit(0);
}

else{//parent process
	printf("for else: %d\n", a[i]);
	close(fd[i][1]);
	read(fd[i][0], a[i], sizeof(int));
	printf("Received string: %d", a[i]);	
}
}
return 0;

}
