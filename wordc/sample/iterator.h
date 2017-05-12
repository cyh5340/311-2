#ifndef ITERATOR_H
#define ITERATOR_H
#include"stdio.h"

typedef struct node* Node;

struct node{
	char *str;
	int num;
	struct node* next;
};

typedef struct LinkedList List;

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
#endif
