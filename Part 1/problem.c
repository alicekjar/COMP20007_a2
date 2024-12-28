/*
    Skeleton code written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1

    Implementation by Alice Kjar
    
    Implementation for module which contains  
        Problem 1-related data structures and 
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
#include <float.h>
#include <math.h>
#include "problem.h"
#include "problemStruct.c"
#include "solutionStruct.c"

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)

#define LDINFINITY (LDBL_MAX / 2.0L)

struct problem;
struct solution;

/* 
    Sets up a solution for the given problem. 
    Written by Grady Fitzpatrick 
*/
struct solution *newSolution(struct problem *problem);

/* Reads a sequence of long doubles in from file
    Written by Grady Fitzpatrick */
void readSequence(FILE *seqFile, int *seqLen, long double **seq);

/* Finds the minimum of 3 long doubles */
long double min(long double a, long double b, long double c);

/* Frees all memory used to store a 3D DTW matrix */
void freeMatrix(long double ***matrix, int n, int m);


/* 
    Reads a sequence of long doubles in from file
    Written by Grady Fitzpatrick 
*/
void readSequence(FILE *seqFile, int *seqLen, long double **seq) {
    char *seqText = NULL;
    /* Read in text. */
    size_t allocated = 0;
    /* Exit if we read no characters or an error caught. */
    int success = getdelim(&seqText, &allocated, '\0', seqFile);

    if(success == -1) {
        /* Encountered an error. */
        perror("Encountered error reading dictionary file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    /* Progress through string. */
    int progress = 0;
    /* Table string length. */
    int seqTextLength = strlen(seqText);

    int commaCount = 0;
    /* Count how many numbers are present. */
    for(int i = 0; i < seqTextLength; i++) {
        if(seqText[i] == ',') {
            commaCount++;
        }
    }
    long double *seqLocal = (long double *) malloc(sizeof(long double) * (commaCount + 1));
    assert(seqLocal);

    int seqAdded = 0;
    while(progress < seqTextLength) {
        int nextProgress;
        /* Read each value into the sequence. */
        assert(sscanf(seqText + progress, "%Lf , %n", &seqLocal[seqAdded], &nextProgress) == 1);
        assert(nextProgress > 0);
        progress += nextProgress;
        seqAdded++;
    }
    assert(seqAdded == (commaCount + 1));
    *seq = seqLocal;
    *seqLen = seqAdded;
    free(seqText);
}

/* 
    Reads in the two sequence files required for problem 1A
    Written by Grady Fitzpatrick
*/
struct problem *readProblemA(FILE *seqAFile, FILE *seqBFile) {
    struct problem *p = (struct problem *) malloc(sizeof(struct problem));
    assert(p);

    int seqALength = 0;
    long double *seqA = NULL;
    readSequence(seqAFile, &seqALength, &seqA);
    int seqBLength = 0;
    long double *seqB = NULL;
    readSequence(seqBFile, &seqBLength, &seqB);

    /* The length of the first sequence. */
    p->seqALength = seqALength;
    /* The first sequence. */
    p->sequenceA = seqA;

    /* The length of the second sequence. */
    p->seqBLength = seqBLength;
    /* The second sequence. */
    p->sequenceB = seqB;

    /* For Part D & F only. */
    p->windowSize = -1;
    p->maximumPathLength = -1;

    p->part = PART_A;

    return p;
}

/* 
    Reads in the two sequence files and limiting window size 
    required for problem 1D
    Written by Grady Fitzpatrick
*/
struct problem *readProblemD(FILE *seqAFile, FILE *seqBFile, int windowSize) {
    /* Fill in Part A sections. */
    struct problem *p = readProblemA(seqAFile, seqBFile);

    p->part = PART_D;
    p->windowSize = windowSize;

    return p;
}

/* 
    Reads in the two sequence files and maximum path length
    required for problem 1F
    Written by Grady Fitzpatrick
*/
struct problem *readProblemF(FILE *seqAFile, FILE *seqBFile, 
    int maxPathLength) {
    /* Interpretation of inputs is same as Part A. */
    struct problem *p = readProblemA(seqAFile, seqBFile);
    
    p->part = PART_F;
    p->maximumPathLength = maxPathLength;

    return p;
}

/*
    Outputs the solution to the given problem. Prints to file the DTW distace
    and matrix
    Written by Grady Fitzpatrick
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName) {
    assert(solution);
    fprintf(outfileName, "%.2Lf\n", solution->optimalValue);
    switch(problem->part) {
        case PART_A:
            assert(solution->matrix);
            for(int i = 1; i <= problem->seqALength; i++) {
                for(int j = 1; j <= problem->seqBLength; j++) {
                    if(solution->matrix[i][j] == LDINFINITY) {
                        fprintf(outfileName, "    ");
                    } else {
                        fprintf(outfileName, "%.2Lf", solution->matrix[i][j]);
                    }
                    if(j < (problem->seqBLength)) {
                        /* Intercalate with spaces. */
                        fprintf(outfileName, " ");
                    }
                }
                fprintf(outfileName, "\n");
            }
            break;
        case PART_D:
        case PART_F:
            break;
    }
}

/*
    Frees the given solution and all memory allocated for it.
    Written by Grady Fitzpatrick
*/
void freeSolution(struct solution *solution, struct problem *problem) {
    if(solution) {
        if(solution->matrix) {
            for(int i = 0; i < problem->seqALength + 1; i++) {
                free(solution->matrix[i]);
            }
            free(solution->matrix);
        }
        free(solution);
    }
}

/*
    Frees the given problem and all memory allocated for it.
    Written by Grady Fitzpatrick
*/
void freeProblem(struct problem *problem) {
    if(problem) {

        if(problem->sequenceA) {
            free(problem->sequenceA);
        }
        if(problem->sequenceB) {
            free(problem->sequenceB);
        }
        free(problem);
    }
}

/* 
    Sets up a solution for the given problem 
    Written by Grady Fitzpatrick
*/
struct solution *newSolution(struct problem *problem) {
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    if(problem->part == PART_F) {
        s->matrix = NULL;
    } else {
        s->matrix = (long double **) malloc(sizeof(long double *) * 
            (problem->seqALength + 1));
        assert(s->matrix);
        for(int i = 0; i < (problem->seqALength + 1); i++) {
            s->matrix[i] = (long double *) malloc(sizeof(long double) * 
                (problem->seqBLength + 1));
            assert(s->matrix[i]);
            for(int j = 0; j < (problem->seqBLength + 1); j++) {
                s->matrix[i][j] = 0;
            }
        }
    }

    s->optimalValue = -1;
    
    return s;
}

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p) {
    struct solution *s = newSolution(p);

    int n = p->seqALength;
    int m = p->seqBLength;

    /* Create and initialise the DTW matrix with infinity */
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            s->matrix[i][j] = LDINFINITY;
        }
    }
    s->matrix[0][0] = 0;

    /* Populate the DTW matrix */
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            /* calculate the distance */
            long double cost = fabsl(p->sequenceA[i-1] - p->sequenceB[j-1]);
            s->matrix[i][j] = cost + min(s->matrix[i-1][j],    // Insertion
                                         s->matrix[i][j-1],    // Deltion
                                         s->matrix[i-1][j-1]   // Match
                                        );
        }
    }
    /* Assign optimal value */
    s->optimalValue = s->matrix[n][m];
    return s;
}

struct solution *solveProblemD(struct problem *p) {
    struct solution *s = newSolution(p);

    int n = p->seqALength;
    int m = p->seqBLength;
    int w = p->windowSize;

    /* Create and initialise the DTW matrix with infinity */
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            s->matrix[i][j] = LDINFINITY;
        }
    }
    s->matrix[0][0] = 0;

    /* Populate the DTW matrix */
    for (int i = 1; i <= n; i++) {
        /* Only consider values in window */
        for (int j = i-w; j <= i+w; j++) {
            if (j < 1 || j > m)
                /* Out  of bounds */
                continue;
            /* Calculate distance */
            long double cost = fabsl(p->sequenceA[i-1] - p->sequenceB[j-1]);
            s->matrix[i][j] = cost + min(s->matrix[i-1][j],    // Insertion
                                         s->matrix[i][j-1],    // Deltion
                                         s->matrix[i-1][j-1]   // Match
                                        );
        }
    }
    /* Assign optimal value */
    s->optimalValue = s->matrix[n][m];
    return s;
}

/*  
    Solves the given problem according to Part F's definition
    and places the solution output into a returned solution value. 
*/
struct solution *solveProblemF(struct problem *p) {
    struct solution *s = newSolution(p);
    
    int n = p->seqALength;
    int m = p->seqBLength;
    int max = p->maximumPathLength;

    /* Allocate heap space for 3D matrix and initialise with infinity */
    long double ***matrix3d = (long double ***)malloc(sizeof(long double **) * (n + 1));
    assert(matrix3d);
    for(int i = 0; i < (n + 1); i++) {
        matrix3d[i] = (long double **) malloc(sizeof(long double *) * (m + 1));
        assert(matrix3d[i]);
        for(int j = 0; j < (m + 1); j++) {
            matrix3d[i][j] = (long double *) malloc(sizeof(long double) * (max + 1));
            assert(matrix3d[i][j]);
            for(int k = 0; k <= max; k++)
                matrix3d[i][j][k] = LDINFINITY;
        }
    }
    matrix3d[0][0][0] = 0;

    /* Iterate through possible path lengths and populate the DTW matrix*/
    for (int step = 1; step <= max; step++) {
        /* Only consider reachable paths */
        for (int i = 1; i <= step && i <= n; i++) {
            for (int j = step - i + 1; j <= step && j <= m; j++) {
                /* Calulate distance */
                long double cost = fabsl(p->sequenceA[i-1] - p->sequenceB[j-1]);
                matrix3d[i][j][step] = cost + min(matrix3d[i-1][j][step-1], // Insertion
                                                  matrix3d[i][j-1][step-1], // Deltion
                                                  matrix3d[i-1][j-1][step-1] // Match
                                                  );

            }
        }
    }

    long double min = matrix3d[n][m][1];
    /* Find minimum for all path lengths within upper bound */
    for (int step = 1; step <= max; step++) {
        if (matrix3d[n][m][step] < min) {
            min = matrix3d[n][m][step];
        }
    }
    freeMatrix(matrix3d, n, m);

    /* Assign optimal value */
    s->optimalValue = min;
    return s;
}

/* Finds the minimum of 3 long doubles */
long double min(long double a, long double b, long double c) {
    return(fminl(a, fminl(b, c)));
}

/* Frees all memory used to store a 3D DTW matrix */
void freeMatrix(long double ***matrix, int n, int m) {
    for(int i = 0; i <= n; i++) {
        for(int j = 0; j <= m; j++) {
            free(matrix[i][j]);
        }
        free(matrix[i]);
    }
    free(matrix);
}

