#include"file.h"
#include"stdlib.h"

char* input(FILE* in)
{
	char *word;
	int i = 0;	// next character for word
	int x = 0;	// multiplier for size of word
	char lttr;	// tempory variable
	lttr = fgetc(in);
	while(!isalnum(lttr) && lttr != EOF)
		lttr = fgetc(in);
	word = malloc(sizeof(char) * MAX);
	do
	{
		x += 1;
		while(lttr != ' ' && lttr != '\n' && lttr != EOF && i < MAX * x )

		{
			if(isalpha(lttr))
			{
				word[i++] = tolower(lttr);
			}
			else if(isdigit(lttr)){
				word[i++] = lttr;
			}
			lttr = fgetc(in);		
		};

		if(i == MAX * x)	
		// if there are possibly more elements (expanding x)
		{
			char* temp = word;
			word = malloc(sizeof(char) * (MAX * x + 1));
			strncpy(word, temp, sizeof(char) * i);
			free(temp);
		}
	}while(i == MAX * x);
	
	if(i == 0)
	{
		free(word);
		word = NULL;
	}
	else
		word[i] = '\0';
	return word;
}
