//name: Crystal Ho
//assignment: 1

#include<stdio.h>
#include<string.h>
#include<stdbool.h>

struct word_t *search_in_list(char *val, struct word_t *prev);
struct word_t {
char *word;
int count;
struct word_t *next;
};

int main(int argc, char *argv[], char *file, char *file2, char*file3) {
FILE *fp1;
char oneword[100];
char *c;
int i=0;
e: Penn State ANGEL: RE: Exam 2 Grades and Reports
int sz = ftell(file);
struct word_t* w= calloc(sz, sizeof(struct word_t));
	
	fp1 = fopen(file ,"r");

	do {
	c = fscanf(fp1,"%s",oneword); /* got one word from the file */
		if (search_in_list(c, w)==NULL){
	      		strcpy(w[i].word,oneword);
			w[i].count=1;
			i++; 
		}
	}while (c != NULL); 

	fclose(fp1);
	//now, for the output
	fopen(file2, "r");
	do{
		printf("%s, %d", w->word, w->count);
	
	}while (w->next!=0);

return 0;
}

struct word_t *search_in_list(char *val, struct word_t *prev){
	struct word_t *ptr = prev;
	struct word_t *tmp = NULL;
	bool found = false;
	while(ptr != NULL) {
		if(ptr->word == val) {
			found = true;
			break;
		}
		else {
		tmp = ptr;
		ptr = ptr->next;
		}
	}
	if(true == found) {
		ptr->count++;
		return ptr;
		}
	else {
		return NULL;
	}
}

