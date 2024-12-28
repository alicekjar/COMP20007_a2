/*
    Skeleton code written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1

    Implementation by Alice Kjar
    
     Implementation for module which contains Prefix Trie 
        data structures and functions.
*/
#include "prefixTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define FALSE 0


/* Creates a new (sub)tree containing the specified string */
struct prefixTree *newPrefixTree(char *word){
    struct prefixTree *retTree = (struct prefixTree *)malloc(sizeof(struct prefixTree));
    assert(retTree);

    for (int i = 0; i < CHARS; i++)
        retTree->next[i] = NULL;
    
    retTree->word = word;
    return retTree;
}

/* Adds a new string to the prefix tree */
struct prefixTree *addWordToTree(struct prefixTree *pt, char *word) {
    struct prefixTree *cur = pt;
    for (int i = 0; word[i]; i++){
        int c = word[i];
        if (cur->next[c]){
            cur = cur->next[c];
        }
        else {
            cur->next[c] = newPrefixTree(NULL);
            cur = cur->next[c];
        }
    }
    cur->next[0] = newPrefixTree(word);
    return pt;
}

