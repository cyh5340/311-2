#include"iterator.h"
#include"stdio.h"
#include<stdlib.h>
#include<string.h>

Node search(List* self, char* c);
Node add(List* self, char* c);
Node addToHead(List* self, char* c);
void print(List* self, FILE* outfile);
void mergesort(List* self);
List mergesortHelper(Node toSort, int length);
int mergeNext(Node* pointerNode, Node* toBeAdded, int* increment);

const List DEFAULT_LIST = {NULL, NULL, 0, 0, 0, &search, &addToHead, &print, &mergesort};
const struct node DEFAULT_NODE = {"", 0, NULL};

//Default constructor for node
Node make_node()
{
	Node newNode = (Node) malloc(sizeof(struct node));
	if(newNode == NULL)
		printf("Out of memory\n");
	if(newNode != NULL)
		*newNode = DEFAULT_NODE;
	return newNode;
}

/*
*  Default constructor for List
*  Every list has a dummy node at its head
*  This is to facilitate easier addition/removal
*   of nodes to the list
*/
List* make_list()
{
	List* newList = (List*) malloc(sizeof(List));
	if(newList != NULL)
	{
		*newList = DEFAULT_LIST;
	
		Node dummy_head = make_node();
		if(dummy_head != NULL)
		{
			dummy_head->str = (char*) malloc(sizeof(char));
			dummy_head->str = "";
	
			newList->head = dummy_head;
		}
	}

	return newList;
}

/*
* Searches for a given string c in a list self
* If the string is not present, it returns a pointer to the location
* that would be immediately preceeding c in the list
*/
Node search(List* self, char* c)
{
	if(!self->sorted)
		self->sort(self);		
	Node curr = self->head;
	while(curr->next != NULL && strcmp(c, curr->next->str) >= 0)
	{
		curr = curr->next;
	}
	
	return curr;
}

/*
*  Adds the given string c to the list self
*  if c is in self, the node corresponding to c has its pointer incremented
*  if c is not in self, the node corresponding to c is added with a num of 1
*/
Node add(List* self, char* c)
{
	Node curr = search(self, c);
	int compare =  strcmp(c, curr->str);
	if(compare == 0)
	{
		(curr->num)++;
	}
	else
	{
		Node temp = make_node();
		if(temp != NULL)
		{	
			temp->str = (char*) malloc((strlen(c) + 1) * sizeof(char));
			if(c != NULL)
				strcpy(temp->str, c);
	        
			temp->num = 1;
		
			temp->next = curr->next;
			curr->next = temp;
		
			(self->numNodes)++;
		}
		curr = temp;
	}
	if(curr != NULL)
		(self->count)++;
	return curr;
}

Node addToHead(List* self, char* c)
{
	Node curr = self->head;
	Node temp = make_node();
	if(temp != NULL)
	{	
		temp->str = (char*) malloc((strlen(c) + 1) * sizeof(char));
		if(c != NULL)
			strcpy(temp->str, c);
        
		temp->num = 1;
	
		temp->next = curr->next;
		curr->next = temp;
	
		(self->numNodes)++;
	}
	curr = temp;
	if(curr != NULL)
		(self->count)++;
	self->sorted = 0;
	return curr;
}

void print(List* self, FILE* outfile)
{
	if(!self->sorted)
		self->sort(self);	
	
	Node curr = self->head;
	while(curr->next != NULL)
	{
		fprintf(outfile, "%s, %d\n", curr->next->str, curr->next->num);
		curr = curr->next;
	}
}

void mergesort(List* self)
{
	List sortedList = mergesortHelper(self->head->next, self->numNodes);
	self->head->next = sortedList.head;
	self->numNodes = sortedList.numNodes;
	self->sorted = 1;
}

List mergesortHelper(Node toSort, int length)
{
	List newList = DEFAULT_LIST;
	if(length <= 1 && toSort != NULL)//Length should never be less than 1, but..
	{
		newList.head = toSort;
		newList.numNodes = 1;
		newList.count = 1;
		return newList;
	}
	else if(toSort == NULL)
	{
		return newList;
	}
	
	newList.curr = toSort;
	Node secondListStart = newList.curr;
	List first;
	List second;
	int firstLength = length/2;
	int i = 0;
	int j=0;
	
	//this needs to be done before merging of the lower list b/c updated pointers
	while(i<firstLength)
	{
		secondListStart = secondListStart->next;
		i++;
	}
	
	first = mergesortHelper(newList.curr, firstLength);
	second = mergesortHelper(secondListStart, length - firstLength);
	
	first.curr = first.head;
	second.curr = second.head;
	
	newList.curr = NULL;
	newList.head = NULL;
	i=0;
	j=0;
	
	while(i< first.numNodes || j<second.numNodes)
	{
		if(i == first.numNodes)//all elements of first have been added
		{
			newList.numNodes += mergeNext(&(newList.curr), &(second.curr), &j);
		}
		else if(j == second.numNodes)//all elements of second have been added
		{
			newList.numNodes += mergeNext(&(newList.curr), &(first.curr), &i);
		}
		else //elements of both lists to be added
		{
			int compare = strcmp(first.curr->str, second.curr->str);
			
			if(compare <= 0)//elements of first are smaller
			{
				newList.numNodes += mergeNext(&(newList.curr), &(first.curr), &i);
			}
			
			
			if(compare > 0)//elements of second are smaller
			{
				newList.numNodes += mergeNext(&(newList.curr), &(second.curr), &j);
			}
		}
		
		if(newList.curr != NULL) 
			newList.curr->next = NULL;
			
		if(newList.head == NULL)
			newList.head = newList.curr;
	}
	newList.count = i + j;
	return newList;
}

int mergeNext(Node* pointerNode, Node* toBeAdded, int* increment)
{
	int nodesAdded = 0;
	if(*pointerNode != NULL && *toBeAdded != NULL)
	{
		int compare = strcmp( (*pointerNode)->str, (*toBeAdded)->str);
		if(compare != 0)
		{
			(*pointerNode)->next = *toBeAdded;
			*pointerNode = *toBeAdded;
			*toBeAdded = (*toBeAdded)->next;
			
			nodesAdded = 1;
		}
		else//duplicate word, just add increment num and free memory
		{
			(*pointerNode)->num += (*toBeAdded)->num;
			Node temp = *toBeAdded;
			*toBeAdded = (*toBeAdded)->next;
			temp->next = NULL;
			free(temp);
		}
	}
	else if(*toBeAdded != NULL)
	{
		*pointerNode = *toBeAdded;
		*toBeAdded = (*toBeAdded)->next;
		
		nodesAdded = 1;
	}
	(*increment)++;
	
	return nodesAdded;
}
