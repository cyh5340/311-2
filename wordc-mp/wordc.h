#ifndef WORDC_H
#define WORDC_H
#include"stdio.h"

typedef struct node* Node;
typedef struct LinkedList List;

struct node{
	char *str;
	int num;
	struct node* next;
};

struct LinkedList{
	Node head;
	Node curr;
	int numNodes;
	int count;
	int sorted;
	Node (*search)(List*, char*);
	Node (*add)(List*, char*);
	void (*print)(List*, FILE*);
	void (*sort)(List*);
};

Node make_node();
List* make_list();
//for the iterator
Node search(List* self, char* c);
Node add(List* self, char* c);
Node addToHead(List* self, char* c);
void print(List* self, FILE* outfile);
void mergeSort(List* self);
List mergeSortHelper(Node toSort, int length);
int mergeNext(Node* pointerNode, Node* toBeAdded, int* increment);




#endif
