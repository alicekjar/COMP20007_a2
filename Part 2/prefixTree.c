/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Implementation for module which contains Prefix Trie 
        data structures and functions.
*/
#include "prefixTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>

#define FALSE 0;

struct prefixTree *newPrefixTree(char *word){
    // malloc space for prefix tree
    // set char to dumm
    // assign all values to NULL
    // 
    struct prefixTree *retTree = (struct prefixTree *)malloc(sizeof(struct prefixTree));
    assert(retTree);

    for (int i = 0; i < 256; i++)
        retTree->next[i] = NULL;
    
    retTree->word = word;
    
    /* Structure of the prefix tree includes a dummy node. */
    //struct prefixTree *retTree = NULL;
    return retTree;
}

//pls rearange this to make neater
struct prefixTree *addWordToTree(struct prefixTree *pt, char *word){
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

