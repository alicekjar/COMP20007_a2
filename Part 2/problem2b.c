/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Make using
        make problem2b
    
    Run using
        ./problem2b dictionary board < text
    
    where dictionary is the name of the file with the 
        dictionary of words in the expected format (e.g. 
        test_cases/2b-1-dict.txt), board is the name
        of the file containing the boggle board, and text 
        is a file containing input text, for example:
    
        ./problem2b test_cases/2b-1-dict.txt test_cases/2b-1-board.txt < test_cases/2b-1-text.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>
#include "problem.h"

#define DICT_FILE_ARG 1
#define BOARD_FILE_ARG 2

/* 
    Runs a program which finds all valid next letters in a partially formed
    Boggle sequence for a particular board/dictionary
    Main function written by Grady Fitzpatrick
*/
int main(int argc, char **argv){
    struct problem *problem;
    struct solution *solution;
    /* Load file with dictionary from argv[1]. */
    FILE *dictFile = NULL;
    /* Load file with table from argv[2]. */
    FILE *boardFile = NULL;

    if(argc < 3){
        fprintf(stderr, "You only gave %d arguments to the program, \n"
            "you should run the program with in the form \n"
            "\t./problem2b dictionary board < text\n", argc);
        return EXIT_FAILURE;
    } 
    
    /* Attempt to open dictionary and board files. */
    dictFile = fopen(argv[DICT_FILE_ARG], "r");
    if(! dictFile){
        fprintf(stderr, "File given as dictionary file was \"%s\", "
            "which was unable to be opened\n", argv[DICT_FILE_ARG]);
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }

    boardFile = fopen(argv[BOARD_FILE_ARG], "r");
    if(! boardFile){
        fprintf(stderr, "File given as board file was \"%s\", "
            "which was unable to be opened\n", argv[BOARD_FILE_ARG]);
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }
    
    problem = readProblemB(dictFile, boardFile, stdin);

    /* Files are no longer needed since they're read in. */
    if(dictFile){
        fclose(dictFile);
    }

    if(boardFile){
        fclose(boardFile);
    }

    solution = solveProblemB(problem);

    outputProblem(problem, solution, stdout);

    freeSolution(solution, problem);

    freeProblem(problem);

    return EXIT_SUCCESS;
}
