/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
     Header for module which contains Problem 1-related 
        data structures and functions.
*/
#include <stdio.h>

struct problem;
struct solution;

/* 
    Reads in the two sequence files required for problem 1A
*/
struct problem *readProblemA(FILE *seqAFile, FILE *seqBFile);

/* 
    Reads in the two sequence files and limiting window size 
    required for problem 1D
*/
struct problem *readProblemD(FILE *seqAFile, FILE *seqBFile, int windowSize);

/*
    Reads in the two sequence files and maximum path length
    required for problem 1F
*/
struct problem *readProblemF(FILE *seqAFile, FILE *seqBFile, int maxPathLength);

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p);

/*
    Solves the given problem according to Part D's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemD(struct problem *p);

/*
    Solves the given problem according to Part F's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemF(struct problem *p);

/*
    Outputs the solution to the given problem. Prints to file the DTW distace
    and matrix
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName);

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem);

/*
    Frees the given problem and all memory allocated for it.
*/
void freeProblem(struct problem *problem);

