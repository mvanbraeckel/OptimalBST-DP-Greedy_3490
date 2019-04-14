/**
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @course CIS*3490: Analysis and Design of Computer Algorithms - A4
 * @version 23/03/2019
 * @file main.c
 * @brief A4 menu to run each different program to solve a problem
 */

// ====================================== INCLUDES ======================================
#include "a4header.h"


int main(int argc, char* argv[]) {

    // declare variables
    char menu_input[3];

    // infinite menu loop, only ending program with '7' as input
    while(1) {
        printf("\n1. P1.c - Optimal Binary Search Tree: Dynamic Programming Technique\n" \
                "2. P2.c - Optimal Binary Search Tree: Greedy Technique\n" \
                "3. Exit\n" \
                "Enter a code (1 – 3) and hit Return\n" \
                "algos/> ");
        fgets(menu_input, 3, stdin);    //only need the first character of input for menu option
        flushInput(menu_input);

        // checks user input
        if(strlen(menu_input) > 1) {
            // display error msg inputting more than just the number option
            printf("\nError: invald input - You must choose one of the menu options by number: [1,3]\nPlease try again\n");

        } else if(menu_input[0] == '1') {         // ================================= 1 =================================
            p1();

        } else if(menu_input[0] == '2') {         // ================================= 2 =================================
            p2();

        } else if(menu_input[0] == '3') {         // ================================= 3 =================================
            printf("\nGood bye!\n");    // display closing msg
            return 0;

        } else {                                  // =============================== ELSE ================================
            // display error msg for bad input
            printf("\nError: invald input - You must choose one of the menu options by number: [1,3]\nPlease try again\n");
        }
    } // end menu loop
}

// ======================================= MY HELPERS =======================================

/**
 * Flushes all leftover data in the stream
 * @param char* input -the string that was just read from stdin
 */
void flushInput(char* input) {
    // if the '\n' is NOT found in the word itself, flush the stream (null-terminate the input regardless)
    if(strchr(input, '\n') == NULL) {
        while ((getchar()) != '\n');
        input[strlen(input)] = '\0';
    } else {
        input[strlen(input)-1] = '\0';
    }
}

// ======================================== HELPERS ========================================

/**
 * Reads in up to 2045 strings from a file
 * @param char* filename -the name of the text file being read
 * @param char* arr[] -the string array of 2045 to be loaded
 * @return the number of unique words in the file
 */
int readStrings(char* filename, char* arr[2045]) {
    char buffer[51] = ""; //50 char max
    int numUniqueWords = 0;

    FILE *fp = fopen(filename, "r");
    // checks if fopen messed up
    if(fp == NULL) {
        fprintf(stderr, "\nERROR: File could not be opened\n\n");

    } else {
        // read one string at a time until the end of the file (or max reached)
        int i = 0;
        while(!feof(fp) && i < 2045) {
            // read a word, allocate mem, copy over data, null terminate (just in case)
            fscanf(fp, " %s ", buffer);

            bool matchFound = false;
            // check if it already exists
            for(int j = 0; j < i; j++) {
                if( strcmp(buffer, arr[j]) == 0 ) {
                    matchFound = true;
                    break;
                }
            }
            // accumulate count if no match was found
            if(!matchFound) {
                numUniqueWords++;
            }

            int bLen = strlen(buffer);
            arr[i] = malloc((bLen+1) * sizeof(char));
            strcpy(arr[i], buffer);
            i++;
        }
    }
    fclose(fp);
    return numUniqueWords;
}

/**
 * Comparator function for QSORT - compares two strings (used for sorting array of strings ascending order)
 * @param a -the first string
 * @param b -the second string
 */
int wordNodeCompar(const void *a, const void *b) {
    WordNode *wordNodeA = (WordNode*)a;
    WordNode *wordNodeB = (WordNode*)b;

    return strcmp( wordNodeA->word, wordNodeB->word );
}

// =================================== FUNCTION HELPERS ====================================

/**
 * Creates a new tree node using a given word node
 * @param toAdd -the word node to be held by the new tree node
 * @param int i, j -the location of it in the main and root table
 * @return -the newly created tree node
 */
TreeNode* newTreeNode(WordNode toAdd, int i, int j) {
    TreeNode* new = malloc(sizeof(TreeNode));
    // copy word node data
    new->wordData.count = toAdd.count;
    new->wordData.freq = toAdd.freq;
    strcpy(new->wordData.word, toAdd.word);
    // add its table location indices
    new->i = i;
    new->j = j;
    // init children
    new->left = NULL;
    new->right = NULL;
    return new;
}

/**
 * Frees the entire tree (all nodes in the tree) by postorder traverse
 * @param root -the tree node to be freed
 */
void freeTree(TreeNode *root) {
    if(root == NULL) {      //recursive base case
        return;
    }
    // print after traversing both left and right subtrees (post)
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
