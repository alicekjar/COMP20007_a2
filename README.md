# COMP20007 - Assignment 2

This assignment was completed as an assessment piece for COMP20007 (Design of Algorithms). It consists of two parts, each of which can be broken down into several sub-problems. Written problems for both parts can be found in the respective pdf files 'written_tasks_p#.pdf'.

Both parts 1 and 2 received full marks upon grading.

## Part 1
Part 1 explored algorithms used in Dynamic Time Warping (DTW) to align two numeric temporal sequences.
### Problem 1A
Provided pseudocode for the DTW algorithm was used to produce a program solving the DTW problem.

- **Input:** Two .txt files each containing a sequence of numbers to be aligned.

- **Output:** The resulting DTW matrix and alignment cost, written to stdout.


### Problem 1B (Written)
A description of the DTW algorithm in comparison to simple sequence matching. The costs of various steps in the algorithm were analysed.

### Problem 1C (Written)
The recurrence relation used in DTW was investigated in relation to dynamic programming.

### Problem 1D
Same as Problem 1A, but introduces a boundary constraint to prevent unrealistic results.

- **Input:** Same as Problem 1A, with the addition of an integer boundary constraint.

- **Output:** Same as Problem 1A.

### Problem 1E (Written)
The computational complexity of the solution to Problem 1D was analysed.

### Problem 1F
Similar to Problem 1A, but with the inclusion of cap on the maximum warping path length. This required the construction of a 3D matrix to dynamically solve the problem.

- **Input:** Same as Problem 1A, with the addition of an integer length cap.

- **Output:** The resulting cumulative distance of the optimal alignment, written to stdout.

### Problem 1G (Written)
The computational complexity of the solution to Problem 1F was analysed.


## Part 2
Part 2 investigated prefix tries through the popular word game [Boggle](https://shop.hasbro.com/en-us/product/boggle/7EB3363D-5056-9047-F5B7-DC51516DFE25).
### Problem 2A
A prefix trie built from a provided dictionary was used to generate all valid words obtainable from a given Boggle board.
- **Input:** 
    - A .txt file containing the number of words in the dictionary (line 1) and all words in the dictionary (all subsequent lines)
    - A .txt file containing the letters which appear on the Boggle board

- **Output:** A list of all valid words, written to stdout.

### Problem 2B
The prefix trie was used to suggest possible next letters for a valid word given a starting sequence.

- **Input:** 
    - A .txt file containing the number of words in the dictionary (line 1) and all words in the dictionary (all subsequent lines)
    - A .txt file containing the letters which appear on the Boggle board
    - A character sequence input through stdin specifying the word constructed so far

- **Output:** A list of all valid next letters, written to stdout.

### Problem 2C
A new solution and algorithm were designed to more efficiently handle Problem 2A on larger boards, under the assumption that a particular letter can only appear once in word (regardless of how many times it appears on the board). The time complexity of this algorithm was analysed in comparison to Problem 2A.


### Problem 2D
The algorithm described in Problem 2C was implemented.

- **Input:** Same as Problem 2A
- **Output:** Same as Problem 2A


## Credits

This assignment built upon skeleton code which was provided by the COMP20007 teaching team and written by Grady Fitzpatrick (code is attributed at a function level). Two basic comparison functions written by [Alistair Moffat](https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/callqsort.c) were used and are attributed. Some algorithmic logic was adapted from pseudocode found in COMP20007 lecture slides and assignment specifications.

## Skills

This project allowed me to develop a number of new/emerging skills, including
- **C** programming
- **Time complexity** analysis
- Complex **problem solving** and **algorithm design**
- **Dynamic Time Warping** algorithms
- **Prefix Trie** data structures

