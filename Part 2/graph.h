/*
    Written by Alice Kjar for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Header for module which contains Boggle Graph 
        data structures and functions.
*/

#ifndef GRAPH_H
#define GRAPH_H

/* Definition of linked list */
struct linkedList;

struct linkedListNode;

struct problem;

/* Defintion of linked list */
struct linkedList {
	struct linkedListNode *head;
	struct linkedListNode *tail;

    char c;
    int visited;
};

/* Definiton of node */
struct linkedListNode {

	struct linkedListNode *next;

	int idx;
};

/* Return a new empty list. */
struct linkedList *newList();


/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, int idx);

/* Free all items in the given list. */
void freeList(struct linkedList *list);

/* Build boggle graph */
struct linkedList **makeGraph(struct problem *p);

/* Find neighbouring elements */
struct linkedList *findNeighbours(int x, int y, struct problem *p, struct linkedList *tile);

/* Frees memory allocated for graph */
void freeGraph(struct linkedList** graph, int n);
#endif
