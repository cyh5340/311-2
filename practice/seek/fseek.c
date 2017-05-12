#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
	char a;
	char oneword;
        FILE* fp;
        pid_t childpid, waitretid;
        int waitarg;
        assert (argc == 2);     
        fp = fopen(argv[1], "r");
        printf ("Parent: before the child seeks, offset is %ld\n", ftell (fp));
	a=scanf(fp);
	     while(!isalnum(a) && a != EOF)
                a = fgetc(fp);

	printf("the word is: %s\n", a);
        childpid = fork ();
	 if (childpid == -1) {
                perror ("Couldn't create child");
                exit(1);
        }
     
	if (childpid >= 0) {
//                fseek (fp, 100, 0);
//		a = fscanf(fp,"%s", oneword); /* got one word from the file */
                printf ("Child: after the child seeks, offset is %ld\n", ftell(fp));
		a=fgetc(fp);
		printf("the word is: %s\n", a);
                exit (0);
        }
        // Parent
        waitretid = wait (&waitarg);
	      if (waitretid == -1) {
                perror ("Wait failed");
                exit (1); 
        }       
        printf ("Parent: after the child seeks, offset is %ld\n", ftell (fp));

}


