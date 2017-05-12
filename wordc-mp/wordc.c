#include"wordc.h"
#include"stdio.h"
#include<stdlib.h>
#include<string.h>


//from the sample for project 1

const List DEFAULT_LIST = {NULL, NULL, 0, 0, 0, &search, &addToHead, &print, &mergeSort};
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

//for inputing
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

//search for counting
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
//to add
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
//to print
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
//to sort
void mergeSort(List* self)
{
	List sortedList = mergeSortHelper(self->head->next, self->numNodes);
	self->head->next = sortedList.head;
	self->numNodes = sortedList.numNodes;
	self->sorted = 1;
}

List mergeSortHelper(Node toSort, int length)
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

	first = mergeSortHelper(newList.curr, firstLength);
	second = mergeSortHelper(secondListStart, length - firstLength);

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
