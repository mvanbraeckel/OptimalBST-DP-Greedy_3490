# Optimal Dynamic Programming & Greedy Binary Search Tree Techniques

* @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
* @course CIS*3490: Analysis and Design of Computer Algorithms - A4
* @version 25/03/2019
* @file README
* @brief Create an optimal and greedy binary search tree from a data file based on word frequency (probability)

## Notes & Assumptions

* Assumption: data files passed are valid and have good formatting, input will have good formatting
* NOTE: the A4 description was not very strict in regards to many things, so if something was NOT explicitly outlined to be required it may not have been implemented. Or if something wasn't specified for how to do something, it may have been done slightly differently due to vagueness.
* NOTE: the A3 guide only recommended to "submit something like...", so I have decided to use this format of README (alternate format can be provided upon request), and I will be submitting a PDF instead of including my anaylsis here (this would be too difficult to use to write all my answers)
* NOTE: Please see my in-code comments for more details
* NOTE: Significant references used have been sourced in my code (if any have been used)

* NOTE: My menu is a menu within a menu for each question: after selecting which problem, it will continuously loop to ask for key to be entered. To stop, enter a blank line
* NOTE: Due to implementation differences, order of words and frequency values for the main table (C[i,j] values for minimum average number of comparisons of the subtree) may differ. This happens while calculating word frequency which affects main table (and in turn root table) construction due to word frequency (probability) rounding error. For instance, I originally used 'double' values, but then my decimals differed from the guide examples (also typing 'final' as key would have 'fire' instead of 'five'). Consequently, I changed my code to use 'long double' values instead, believing my precision was not good enough; however, it still didn't help. Afterwards, I found that by using 'float' values instead, I matched the professors output.
* NOTE: I formatted my output to make it easier to read, I also included 6 decimal places to make lower frequency numbers more easily differentiable
* NOTE: I've left some commented out code blocks that are mainly print statements that helped me check and debug as I worked (left them in case I want to see again later, but this should be fine in regards to not hindering documentation)

## makefile, then Run

My files: README.md, makefile, a4header.h, main.c, P1.c, P2.c, and data_7.txt

_NOTE: make sure a bin folder exists before compiling using "make"_
To compile: type "make" --all *.o files and the executable file "run" will be located in a bin folder

## main.c

Usage: ./bin/run

> You will see a menu come up, where you can choose which of the 4 questions to run, or exit. For example:

```
1. P1.c - Optimal Binary Search Tree: Dynamic Programming Technique
2. P2.c - Optimal Binary Search Tree: Greedy Technique
3. Exit
Enter a code (1 – 3) and hit Return
algos/>
```

* NOTE: entering '3' will exit the program with a "Goodbye!" msg
* NOTE: typing bad input will give an error message

### P1.c

Option 1: Optimal Binary Search Tree: Dynamic Programming Technique - Example input, output:

```
algos/> 1

P1 - Optimal BST: Dynamic Programming Technique (enter a blank line to exit)
Enter a key: undergraduate
Compared with '              of' (6.147185), go right subtree.
Compared with '             the' (1.972126), go right subtree.
Compared with '      university' (0.412225), go left subtree.
Compared with '              to' (0.155990), go right subtree.
Compared with '   undergraduate' (0.052812), found.

Enter a key:
```

* NOTE: will continue loops the P1 menu with only the "Enter a key: " prompt. --> TO EXIT: enter a blank line/key

### P2.c

Option 2: Optimal Binary Search Tree: Greedy Technique - Example input, output:

```
algos/> 2

P2 - Optimal BST: Greedy Technique (enter a blank line to exit)
Enter a key: wine
Compared with '             the' (0.061125), go right subtree.
Compared with '              to' (0.030807), go right subtree.
Compared with '      university' (0.018093), go right subtree.
Compared with '            with' (0.005379), go left subtree.
Compared with '           which' (0.003912), go right subtree.
Compared with '          winter' (0.001956), go left subtree.
Compared with '            will' (0.001467), go right subtree.
Not found.

Enter a key:
```
