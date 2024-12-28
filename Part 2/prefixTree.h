/*
    Skeleton code written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1

    Implementation by Alice Kjar
    
     Header for module which contains Prefix Trie 
        data structures and functions.
*/
struct prefixTree;

#ifndef PREFIXTREESTRUCT
#define PREFIXTREESTRUCT
#define CHILD_COUNT (1 << 8)
#define UNMARKED (0)
#define MARKED (1)
#define CHARS 256

struct prefixTree;

/* Prefix tree data structure */
struct prefixTree {
    struct prefixTree *next[CHARS];
    char *word;
};

#endif

/* Creates a new (sub)tree containing the specified string */
struct prefixTree *newPrefixTree(char *word);

/* Adds a new string to the prefix tree */
struct prefixTree *addWordToTree(struct prefixTree *pt, char *word);

