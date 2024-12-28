/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
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

struct prefixTree {
    struct prefixTree *next[CHARS];
    char *word;
};

#endif

struct prefixTree *newPrefixTree(char *word);

struct prefixTree *addWordToTree(struct prefixTree *pt, char *word);

