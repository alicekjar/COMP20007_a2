/*
    Written by Alice Kjar for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Program to build adjacency list for boggle graph
*/
#include "graph.h"
#include "problemStruct.c"
#include <assert.h>
#include <stdlib.h>

/* Return a new empty list. */
struct linkedList *newList() {
	/* assign memory */
	struct linkedList *list;
	list = (struct linkedList*)malloc(sizeof(*list));
	assert(list);

	/* initialise pointers */
	list->head = NULL;
	list->tail = NULL;
    list->c = 0;
    list->visited = 0;

	return list;
}

/* Insert the given x, y pair at the tail of the list */
void insertTail(struct linkedList *list, int idx) {
	/* check we have valid list */
	assert(list);

	/* make new node */
	struct linkedListNode *new_node;
	new_node = (struct linkedListNode*)malloc(sizeof(*new_node));
	assert(new_node);
	new_node->idx = idx;
	new_node->next = NULL;

	if (list->tail) {
		/* list non-empty */
		list->tail->next = new_node;
	}
	else {
		list->head=new_node;
	}
	/* reset list tail */
	list->tail = new_node;
}

/* Free all items in the given list. */
void freeList(struct linkedList *list) {
	struct linkedListNode *cur = list->head;
	struct linkedListNode *next;

	/* iterate through linked list to free all elems */
	while (cur) {
		next = cur->next;
		free(cur);
		cur = next;
	}

	/* free list itself */
	free(list);
	list = NULL;
}


/* Builds boggle graph from board */
struct linkedList **makeGraph(struct problem *p){
    int n = p->dimension;
    struct linkedList **chars = (struct linkedList **)malloc(sizeof(struct linkedList*) * n*n);
    assert(chars);

	/* look at each element in graph */
    for (int y = 0; y < n; y++){
        for (int x = 0; x < n; x++){
            chars[n*y+x] = findNeighbours(x, y, p, chars[n*y+x]);
        }
    }
    return chars;
}

/* Finds neighbouring elements in board and adds to boggle graph */
struct linkedList *findNeighbours(int x, int y, struct problem *p, struct linkedList *tile){
    tile = newList();
    tile->c = p->board[y][x];

    int n = p->dimension;
    /* Iterate through neighbours */
    for (int i = -1; i <= 1; i++) {
        if (y + i < 0 || y + i > n - 1) {
            /* Out of range */
            continue;
        }
        for (int j = -1; j <= 1; j++){
            if (x + j < 0 || x + j > n - 1){
                /* Out of range */
                continue;
            }
			/* Don't need to include connections between two of the same letters
			in part D */
			if (p->part != PART_D || tile->c != p->board[y+i][x+j])
            	insertTail(tile, n*(y+i)+x+j);
        }
    }
    return tile;
}

/* Frees memory allocated for graph */
void freeGraph(struct linkedList** graph, int n) {
	for (int i = 0; i < n*n; i ++){
		freeList(graph[i]);
	}
	free(graph);
	graph = NULL;
}

