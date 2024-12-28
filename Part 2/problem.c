/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Implementation for module which contains  
        Problem 2-related data structures and 
        functions.
    
    Sample solution implemented by Grady Fitzpatrick
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "problem.h"
#include "problemStruct.c"
#include "solutionStruct.c"
#include "prefixTree.h"
#include "graph.h"

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)

#define FALSE 0
#define TRUE 1

#define MAX_NEAR 9
#define CHARS 256

struct problem;
struct solution;

/* Sets up a solution for the given problem. */
struct solution *newSolution(struct problem *problem);

/* Performs dfs to find valid words */
void dfs(int idx, struct linkedList **graph, struct solution *s, struct prefixTree **cur, int n);

/* Adds word to solution */
void addWord(char *word, struct solution *s);

/* string comparison function */
int string_ascending(const void *v1, const void *v2);

/* finds valid position of partial string on graph */
void findStart(int idx, struct linkedList **graph, struct solution *s, char *part, struct prefixTree **cur, int n);

/* checks each of the neighbouring pieces after partial string */
void checkNeighbours(int idx, struct linkedList **graph, struct solution *s, struct prefixTree **cur,int n);

/* finds if position can generate a valid suffix on graph */
int dfs2(int idx,struct linkedList **graph, struct prefixTree **cur, int n);

/* Adds character to solution */
struct solution *insertChar(struct solution *s, char c);

/* character comparison function */
int char_ascending(const void *v1,const void *v2);

/* checks all possible ways of reaching a node on prefix tree at once */
void checkLevel(struct linkedList **graph, struct solution *s, struct prefixTree **cur, int level, int n, int *seen);


/* 
    Reads the given dict file into a list of words 
    and the given board file into a nxn board.
*/
struct problem *readProblemA(FILE *dictFile, FILE *boardFile) {
    struct problem *p = (struct problem *) malloc(sizeof(struct problem));
    assert(p);

    /* Part B onwards so set as empty. */
    p->partialString = NULL;

    int wordCount = 0;
    int wordAllocated = 0;
    char *dictText = NULL;
    char **words = NULL;

    /* Read in text. */
    size_t allocated = 0;
    /* Exit if we read no characters or an error caught. */
    int success = getdelim(&dictText, &allocated, '\0', dictFile);

    if(success == -1) {
        /* Encountered an error. */
        perror("Encountered error reading dictionary file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    char *boardText = NULL;
    /* Reset allocated marker. */
    allocated = 0;
    success = getdelim(&boardText, &allocated, '\0', boardFile);

    if(success == -1) {
        /* Encountered an error. */
        perror("Encountered error reading board file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    /* Progress through string. */
    int progress = 0;
    /* Table string length. */
    int dictTextLength = strlen(dictText);

    /* Count words present. */
    while(progress < dictTextLength) {
        char *word = NULL;
        int nextProgress;
        /* Read each value into the dictionary. */
        if(progress == 0) {
            /* First line. */
            int wordNumberGuess;
            assert(sscanf(dictText + progress, "%d %n", &wordNumberGuess, &nextProgress) == 1);
            /* Will fail if integer missing from the start of the words. */
            assert(nextProgress > 0);
            if(wordNumberGuess > 0) {
                wordAllocated = wordNumberGuess;
                words = (char **) malloc(sizeof(char *) * wordAllocated);
                assert(words);
            }
            progress += nextProgress;
            continue;
        } else {
            assert(sscanf(dictText + progress, "%m[^\n] %n", &word, &nextProgress) == 1);
            assert(nextProgress > 0);
            progress += nextProgress;
        }

        /* Check if more space is needed to store the word. */
        if(wordAllocated <= 0) {
            words = (char **) malloc(sizeof(char *) * INITIALWORDSALLOCATION);
            assert(words);
            wordAllocated = INITIALWORDSALLOCATION;
        } else if(wordCount >= wordAllocated) {
            words = (char **) realloc(words, sizeof(char *) * 
                wordAllocated * 2);
            assert(words);
            wordAllocated = wordAllocated * 2;
            /* Something has gone wrong if there's not sufficient word 
                space for another word. */
            assert(wordAllocated > wordCount);
        }

        words[wordCount] = word;
        wordCount++;
    }
    
    /* Done with dictText */
    if(dictText) {
        free(dictText);
    }
    
    /* Now read in board */
    progress = 0;
    int dimension = 0;
    int boardTextLength = strlen(boardText);
    /* Count dimension with first line */
    while(progress < boardTextLength) {
        /* Count how many non-space characters appear in line. */
        if(boardText[progress] == '\n' || boardText[progress] == '\0') {
            /* Reached end of line. */
            break;
        }
        if(isalpha(boardText[progress])) {
            dimension++;
        }
        progress++;
    }

    assert(dimension > 0);

    /* Check each line has the correct dimension. */
    for(int i = 1; i < dimension; i++) {
        int rowDim = 0;
        if(boardText[progress] == '\n') {
            progress++;
        }
        while(progress < boardTextLength) {
            /* Count how many non-space characters appear in line. */
            if(boardText[progress] == '\n' || boardText[progress] == '\0') {
                /* Reached end of line. */
                break;
            }
            if(isalpha(boardText[progress])) {
                rowDim++;
            }
            progress++;
        }
        if(rowDim != dimension) {
            fprintf(stderr, "Row #%d had %d letters, different to Row #1's %d letters.\n", i + 1, rowDim, dimension);
            assert(rowDim == dimension);
        }
    }

    /* Define board. */
    char *boardFlat = (char *) malloc(sizeof(char) * dimension * dimension);
    assert(boardFlat);
    
    /* Reset progress. */
    progress = 0;
    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < dimension; j++) {
            int nextProgress;
            assert(sscanf(boardText + progress, "%c %n", &boardFlat[i * dimension + j], &nextProgress) == 1);
            progress += nextProgress;
        }
    }

    char **board = (char **) malloc(sizeof(char **) * dimension);
    assert(board);
    for(int i = 0; i < dimension; i++) {
        board[i] = &boardFlat[i * dimension];
    }

    // fprintf(stderr, "\n");

    /* The number of words in the text. */
    p->wordCount = wordCount;
    /* The list of words in the dictionary. */
    p->words = words;

    /* The dimension of the board (number of rows) */
    p->dimension = dimension;

    /* The board, represented both as a 1-D list and a 2-D list */
    p->boardFlat = boardFlat;
    p->board = board;

    /* For Part B only. */
    p->partialString = NULL;

    p->part = PART_A;

    if(boardText){
        free(boardText);
    }

    return p;
}

struct problem *readProblemB(FILE *dictFile, FILE *boardFile, 
    FILE *partialStringFile) {
    /* Fill in Part A sections. */
    struct problem *p = readProblemA(dictFile, boardFile);

    char *partialString = NULL;

    /* Part B has a string that is partially given - we assume it follows 
        word conventions for the %s specifier. */
    assert(fscanf(partialStringFile, "%ms", &partialString) == 1);
    
    p->part = PART_B;
    p->partialString = partialString;

    return p;
}

struct problem *readProblemD(FILE *dictFile, FILE *boardFile) {
    /* Interpretation of inputs is same as Part A. */
    struct problem *p = readProblemA(dictFile, boardFile);
    
    p->part = PART_D;
    return p;
}

/*
    Outputs the given solution to the given file. If colourMode is 1, the
    sentence in the problem is coloured with the given solution colours.
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName) {
    assert(solution);
    switch(problem->part) {
        case PART_A:
        case PART_D:
            assert(solution->foundWordCount == 0 || solution->words);
            for(int i = 0; i < solution->foundWordCount; i++) {
                fprintf(outfileName, "%s\n", solution->words[i]);
            }
            break;
        case PART_B:
            assert(solution->foundLetterCount == 0 || solution->followLetters);
            for(int i = 0; i < solution->foundLetterCount; i++) {
                if(isalpha(solution->followLetters[i])) {
                    fprintf(outfileName, "%c\n", solution->followLetters[i]);
                } else {
                    fprintf(outfileName, " \n");
                }
            }
            break;
    }
}

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem) {
    if(solution) {
        if(solution->followLetters) {
        free(solution->followLetters);
        }
        if(solution->words) {
            free(solution->words);
        }
        free(solution);
    }
}

/*
    Frees the given problem and all memory allocated for it.
*/
void freeProblem(struct problem *problem) {
    if(problem) {
        if(problem->words) {
            for(int i = 0; i < problem->wordCount; i++) {
                if(problem->words[i]) {
                    free(problem->words[i]);
                }
            }
            free(problem->words);
        }
        if(problem->board) {
            free(problem->board);
        }
        if(problem->boardFlat) {
            free(problem->boardFlat);
        }
        if(problem->partialString) {
            free(problem->partialString);
        }
        free(problem);
    }
}

/* Sets up a solution for the given problem */
struct solution *newSolution(struct problem *problem) {
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->foundWordCount = 0;
    s->nAlloc = 0;
    s->words = NULL;
    s->foundLetterCount = 0;
    s->followLetters = NULL;
    
    return s;
}

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p) {
    struct solution *s = newSolution(p);

    /* make the prefix tree */
    struct prefixTree *tree = newPrefixTree(NULL);
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    int n = p->dimension;

    /* make the boggle graph */
    struct linkedList **graph = makeGraph(p);

    /* iterate through all starting positions */
    for (int i = 0; i < n*n; i++) {
        int c = tolower(graph[i]->c);

        if (tree->next[c]) {
            /* Found a possible starting letter */
            dfs(i, graph, s, tree->next[c]->next, n);
        }
    }

    /* sort strings */
    qsort(s->words, s->foundWordCount, sizeof(char*), string_ascending);

    freeGraph(graph, n);
    return s;
}

/* 
    String comparision function, written by Alistair Moffat
    https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/callqsort.c
*/
int string_ascending(const void *v1, const void *v2) {
	return strcmp(*(char**)v1,*(char**)v2);
}


/* 
    Character comparision function, adapted from
    https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/funcarg.c
*/
int char_ascending(const void *v1,const void *v2) {
	const char *c1=v1, *c2=v2;
	if (*c1<*c2) return -1;
	if (*c1>*c2) return +1;
	return 0;
}

/* 
    Dynamically adds a new word to the solution.
    Code has been adapted from skeleton code
*/
void addWord(char *word, struct solution *s) {
    /* Check if more space is needed to store the word. */
    if(s->nAlloc <= 0) {
        s->words = (char **) malloc(sizeof(char *) * INITIALWORDSALLOCATION);
        assert(s->words);
        s->nAlloc = INITIALWORDSALLOCATION;
    } 
    else if(s->foundWordCount >= s->nAlloc) {
        s->words = (char **) realloc(s->words, sizeof(char *) * s->nAlloc * 2);
        assert(s->words);
        s->nAlloc *= 2;
        /* Something has gone wrong if there's not sufficient word 
            space for another word. */
        assert(s->nAlloc > s->foundWordCount);
    }
    /* We have enough space, add new word */
    s->words[s->foundWordCount] = word;
    s->foundWordCount++;
}


/* Recursively performs DFS on the Boggle Graph, finding all possible words */
void dfs(int idx, struct linkedList **graph, struct solution *s, struct prefixTree **cur, int n) {
    if (cur['\0'] && cur['\0']->word != NULL) {
        /* Found the end of a word */
        addWord(cur[0]->word, s);
        /* Don't want to find word again */
        cur['\0']->word = NULL;
    }

    graph[idx]->visited = TRUE;

    /* Iterate through neighbours */
    struct linkedListNode *near = graph[idx]->head;
    while(near != NULL) {
        int c = tolower(graph[near->idx]->c);

        if (!graph[near->idx]->visited && cur[c] != NULL) {
            /* Found a viable prefix */
            dfs(near->idx, graph, s, cur[c]->next, n);
        }
        near = near->next;
    }

    /* Found all possible words, backtrack */
    graph[idx]->visited = FALSE;
}


/*
    Solves the given problem according to Part B's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemB(struct problem *p) {
    struct solution *s = newSolution(p);
    
    /* make the prefix tree */
    struct prefixTree *tree = newPrefixTree(NULL);
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    int n = p->dimension;
    
    /* make the boggle graph */
    struct linkedList **graph = makeGraph(p);

    /* check that partial is valid */
    struct prefixTree *cur = tree;
    for (int i = 0; p->partialString[i]; i++) {
        if (!cur) {
            return s;
        }
        cur = cur->next[(int)p->partialString[i]];
    }
    if (cur->next[0] != NULL) {
        /* partial is a word itself */
        insertChar(s, ' ');
    }

    /* iterate through all starting positions */
    for (int i = 0; i < n*n; i++) {
        int c = tolower(graph[i]->c);
        if (c == p->partialString[0] && tree->next[c]) {
            /* Found a possible starting letter */
            findStart(i, graph, s, p->partialString + 1, tree->next[c]->next, n);
        }
    }

    /* sort characters */
    qsort(s->followLetters, s->foundLetterCount, sizeof(char), char_ascending);
    
    freeGraph(graph, n);
    return s;
}


/* 
    Recursively moves through boggle graph and partial string simultaneously, 
    finding where partial occurs on graph and dfs can begin 
*/
void findStart(int idx, struct linkedList **graph, struct solution *s, 
                char *part, struct prefixTree **cur, int n) {

    if (!part[0]) {
        /* reached end of the partial, time to start checking */
        checkNeighbours(idx, graph, s, cur, n);
    }
    
    graph[idx]->visited = TRUE;

    /* Iterate through neighbours */
    struct linkedListNode *near = graph[idx]->head;
    while(near != NULL) {
        int c = tolower(graph[near->idx]->c);

        if (c == part[0] && !graph[near->idx]->visited) {
            /* Found a viable prefix, check next letter */
            findStart(near->idx, graph, s, part + 1, cur[c]->next, n);
        }
        near = near->next;
    }

    /* Found all possible occurences of partial, backtrack */
    graph[idx]->visited = FALSE;
}


/* 
    Iterates through each of the neighbouring letters of the given idx on the graph
    and checks if it can produce a valid word
*/
void checkNeighbours(int idx, struct linkedList **graph, struct solution *s, struct prefixTree **cur,int n) {

    /* Iterate through neighbours */
    struct linkedListNode *near = graph[idx]->head;
    while(near != NULL) {
        int c = tolower(graph[near->idx]->c);

        if (!graph[near->idx]->visited && cur[c]) {
            /* Check if word can be made */
            if (dfs2(near->idx, graph, cur[c]->next, n)) {
                s = insertChar(s, c);
            }
        }
        near = near->next;
    }
}


/* 
    Modified version of dfs which recursively performs DFS on the Boggle Graph
    starting from a given point, returning immediately if a word can be made
*/
int dfs2(int idx,struct linkedList **graph, struct prefixTree **cur, int n) {
    if (cur['\0'] && cur['\0']->word != NULL) {
        /* Found a word! */
        return TRUE;
    }

    graph[idx]->visited = TRUE;

    /* Iterate through neighbours */
    struct linkedListNode *near = graph[idx]->head;
    while(near != NULL) {
        int c = tolower(graph[near->idx]->c);
        if (!graph[near->idx]->visited && cur[c]) {
            /* Found a viable prefix, check next letter */
            if (dfs2(near->idx, graph, cur[c]->next, n)) {
                /* valid word found */
                graph[idx]->visited = FALSE;
                return TRUE;
            }
        }
        near = near->next;
    }

    /* Found no possible words, backtrack */
    graph[idx]->visited = FALSE;
    return FALSE;
}


/* Inserts valid next letter into solution */
struct solution *insertChar(struct solution *s, char c) {
    if (s->foundLetterCount == 0) {
        /* Need to allocate memory */
        s->followLetters = (char *) malloc(sizeof(char) * MAX_NEAR);
        assert(s->followLetters);
    }
    for (int i = 0; i < s->foundLetterCount; i++) {
        /* check for duplicates */
        if (s->followLetters[i] == c)
            return s;
    }
    s->followLetters[s->foundLetterCount] = c;
    s->foundLetterCount++;
    return s;
}

/*
    Solves the given problem according to Part D's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemD(struct problem *p) {
    struct solution *s = newSolution(p);

    /* make the prefix tree */
    struct prefixTree *tree = newPrefixTree(NULL);
    for (int i = 0; i < p->wordCount; i++) {
        tree = addWordToTree(tree, p->words[i]);
    }

    /* intialise array of visited characters */
    int seen[CHARS] = { FALSE };

    int n = p->dimension;
    struct linkedList **graph = makeGraph(p);

    /* iterate through all nodes in first level of tree */
    /* Note that efficiency could be improved if nodes of our prefix tree were 
    stored in a linked list (rather than an array) */
    for (int c = 0; c < CHARS; c++) {
        if (tree->next[c] != NULL) {
            /* found a valid starting letter, check if in board */
            for (int i = 0; i < n*n; i++) {
                int start = tolower(graph[i]->c);

                if (c == start) {
                    /* Mark as possible first letter */
                    graph[i]->visited = 1;
                    seen[c] = TRUE;
                }
            }
            /* check all possible positions for given letter */
            checkLevel(graph, s, tree->next[c]->next, 1, n, seen);

            /* reset */
            seen[c] = FALSE;
            for (int i = 0; i < n*n; i++) {
                graph[i]->visited = FALSE;
            }
        }
    }
    
    /* sort strings */
    qsort(s->words, s->foundWordCount, sizeof(char*), string_ascending);

    freeGraph(graph, n);
    return s;
}


/*
    Recursively moves through each node of tree and performs bfs to 
    find all valid spots on graph for the given node
*/
void checkLevel(struct linkedList **graph, struct solution *s, struct prefixTree **cur, int level, int n, int *seen) {
    /* iterate through all nodes in this level of tree */
    /* Note that efficiency could be improved if nodes of our prefix tree were 
    stored in a linked list (rather than an array) */
    for (int c = 0; c < CHARS; c++) {
        if (cur[c] != NULL && !seen[c]) {
            int found_char = FALSE;
            for (int i = 0; i < n*n; i++) {
                /* found a valid next letter, check if in board */
                if (graph[i]->visited == level) {
                    /* we can reach this position on the board by following the path traversed on the tree */
                    /* iterate through neighbours */
                    struct linkedListNode *tile = graph[i]->head;
                    while(tile != NULL) {
                        int piece = tolower(graph[tile->idx]->c);

                        if (!graph[tile->idx]->visited && piece == c) {
                            /* this tile lets us get to the selected node */
                            found_char = TRUE;
                            seen[c] = TRUE;
                            graph[tile->idx]->visited = level + 1;

                            if (cur[c]->next[0] && cur[c]->next[0]->word != NULL) {
                                /* Found the end of a word */
                                addWord(cur[c]->next[0]->word, s);
                                cur[c]->next[0]->word = NULL;     // Prevents repeats
                            }
                        }
                        
                    tile = tile->next;
                    } 
                }
            }

            if (found_char) {
                /* check for next set of nodes */
                checkLevel(graph, s, cur[c]->next, level + 1, n, seen);
            }
            seen[c] = FALSE;
        }
    }

    /* reset */
    for (int i = 0; i < n*n; i++) {
        if (graph[i]->visited == level)
            graph[i]->visited = FALSE;
    }
}

