/**
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @course CIS*3490: Analysis and Design of Computer Aforithms - A4
 * @version 23/03/2019
 * @file P1.c
 * @brief A4 Problem 1 - optimal binary search tree: dynamic programming technique
 * REFERENCES: textbook, various sources and videos on internet, geeks for geeks
 * NOTE: I wish I went to classes or labs recently so I already would know how to do this instead of teach mysef last minute :(
 */

#include "a4header.h"


void p1() {
    // declare variables
    char user_input[51] = ""; // 50 char max
    char* strArr[2045];

    // read in the data
    int numUniqueWords = readStrings("data_7.txt", strArr);
    int totalWords = sizeof(strArr) / sizeof(char*);

    /*char* strArr[10] = { "A", "B","B", "C","C","C","C", "D","D","D" };
    int numUniqueWords = 4;
    int totalWords = 10;*/

    /*// print all words
    for(int i = 0; i < totalWords; i++) {
        printf("\tword: '%s'\n", strArr[i]);
    }
    printf("totalWords = %d\n", totalWords); // 2045 total words
    printf("numUniqueWords = %d\n", numUniqueWords);*/ // 600 unique words

    // create a WordNode array, init all values
    WordNode wordArr[numUniqueWords];
    for(int i = 0; i < numUniqueWords; i++) {
        wordArr[i].count = 0;
        wordArr[i].freq = 0.0;
        strcpy(wordArr[i].word, "");
    }
    
    // loop through the word array using it to populate the WordNode array
    int index = 0;
    for(int i = 0; i < totalWords; i++) {
        bool matchFound = false;
        // check if it already exists in the WordNode array, accumulate its count if it does
        for(int j = 0; j < index; j++) {
            if( strcmp(strArr[i], wordArr[j].word) == 0 ) {
                matchFound = true;
                wordArr[j].count++;
                break;
            }
        }
        // otherwise, add it as a new word
        if(!matchFound) {
            wordArr[index].count = 1;
            strcpy(wordArr[index].word, strArr[i]);
            wordArr[index].word[strlen(strArr[i])] = '\0';
            index++;
        }
        free(strArr[i]); // don't need anymore
    }

    // sort it alphabetically (ascending)
    qsort(wordArr, numUniqueWords, sizeof(WordNode), *wordNodeCompar);

    // calc frequency (probability) of each word, and display it
    for(int i = 0; i < numUniqueWords; i++) {
        wordArr[i].freq = (float)wordArr[i].count / (float)totalWords;
        /*if(strcmp(wordArr[i].word, "five") == 0 || strcmp(wordArr[i].word, "fire") == 0 || strcmp(wordArr[i].word, "final") == 0) {
            printf("\tword: '%20s' = %3d (%3.6f) | i = %d\n", wordArr[i].word, wordArr[i].count, wordArr[i].freq, i);
        }*/
    }

    // create two matrices for C and R (main table and root table)
    int numRows = numUniqueWords+2; //start at index 1, ignore first 0th row
    int numCols = numUniqueWords+1;

    float **mainTable = (float**)malloc(numRows * sizeof(float*));
    int **rootTable = (int**)malloc(numRows * sizeof(int*));
    for(int i = 0; i < numRows; i++) {
        mainTable[i] = (float*)calloc(numCols, sizeof(float));
        rootTable[i] = (int*)calloc(numCols, sizeof(int));
    }
    
    // calc and fill the tables appropriately
    constructTables(mainTable, rootTable, wordArr, numUniqueWords);

    // display both tables
    /*printf("\n------------------------------------------------------------ MAIN TABLE ------------------------------------------------------------\n");
    for(int i = 1; i < numRows; i++) {
        printf("\n");
        for(int j = 0; j < numCols; j++) {
            printf("%8.6f ", mainTable[i][j]);
        }
    }
    printf("\n\n\n------------------------------------------------------------ ROOT TABLE ------------------------------------------------------------\n");
    for(int i = 1; i < numRows; i++) {
        printf("\n");
        for(int j = 0; j < numCols; j++) {
            printf("%3d ", rootTable[i][j]);
        }
    }*/

    // create the actual optimal BST
    TreeNode *root = createOptimalBST(wordArr, rootTable, numUniqueWords);

    // infinite menu loop, only ending program with '7' as input
    printf("\nP1 - Optimal BST: Dynamic Programming Technique (enter a blank line to exit)");
    while(1) {
        printf("\nEnter a key: ");
        fgets(user_input, 51, stdin);
        flushInput(user_input);

        // checks user input if they want to exit
        if(user_input[0] == '\0') {
            printf("\nGood bye! --leaving P1: Optimal BST: Dynamic Programming Technique\n");    // display closing msg

            // free tables first before leaving
            for(int i = 0; i < numRows; i++) {
                free(mainTable[i]);
                free(rootTable[i]);
            }
            free(mainTable);
            free(rootTable);

            // free tree before leaving 
            freeTree(root);

            return;

        } else {                                  // =============================== ELSE ================================
            // search the tree
            int keyFound = 0;
            searchOptimalBST(root, mainTable, user_input, &keyFound);
            if(!keyFound) {
                printf("Not found.\n");
            }
        }
    } // end menu loop
} // end main

// ===============================================================================================

/**
 * Creates the main and root tables for the optimal BST (dynamic programming technique)
 * @param float **mainTable -matrix to be filled with minimum average number of comparisons (catalan number)
 * @param int **rootTable -matrix filled with index corresponsing to location in word node array
 * @param WordNode *wordArr -alphabetically sorted (ascending) array of word nodes from the file data
 * @param int numUniqueWords -#of unique words in the file
 */
void constructTables(float **mainTable, int **rootTable, WordNode *wordArr, int numUniqueWords) {
    // init both tables
    for(int i = 1; i <= numUniqueWords; i++) {
        mainTable[i][i-1] = 0.0;
        mainTable[i][i] = wordArr[i-1].freq;
        rootTable[i][i] = i;
    }

    // loop through the table on the diagonal (down-right each step)
    for(int d = 1; d <= numUniqueWords-1; d++) {
        for(int i = 1; i <= numUniqueWords-d; i++) {
            int j = d + i;
            
            // calc summation of probabilities (textbook)
            float sum = 0;
            for(int s = i-1; s <= j-1; s++) {
                sum += wordArr[s].freq;
            }
            
            // find the minimum of all i to j (textbook), reset min placeholders tp max values first
            float min = DBL_MAX;
            int k_min = INT_MAX;
            for(int k = i; k <= j; k++) {
                // calc the current catalan number i.e. C(i,j)
                float catalan = mainTable[i][k-1] + mainTable[k+1][j];

                // reset minimum placeholders
                if(catalan < min) {
                    min = catalan;
                    k_min = k;
                }
            }

            // now load the proper min values into each table
            mainTable[i][j] = min + sum;
            rootTable[i][j] = k_min;
        }
    }
}

/**
 * Creates optimal BST from root table
 * @param WordNode *wordArr -array of word nodes from the file
 * @param int **rootTable -matrix filled out already
 * @param int numUniqueWords -#of unique words from the file
 * @return the root of the created optimal BST
 */
TreeNode* createOptimalBST(WordNode *wordArr, int **rootTable, int numUniqueWords) {
    // create an optimal BST (the root node)
    TreeNode* root = NULL;
    // NOTE: index corresponsing to wordArr is rootValue-1 -- NOTE: numCols = numUniqueWords+1
    // start by grabbing top-right index (we start row from 1, and last column)
    root = insertFromRootTable(root, wordArr, rootTable, 1, numUniqueWords); // inclusive left and right indices
    return root;
}

/**
 * Adds a word node to the optimal BST
 * @param TreeNode *root -the subtree being looked at
 * @param WordNode *wordArr -the array of word nodes from the file
 * @param int **rootTable -the matrix filled out already
 * @param int left -inclusive left-side start of the sub array of wordArr
 * @param int right -inclusive right-side end of the sub array of wordArr
 * @return -the (new) optimal BST
 */
TreeNode* insertFromRootTable(TreeNode *root, WordNode *wordArr, int **rootTable, int left, int right) {
    // continue inserting word nodes into the tree until trying to add into left > right [left,right] (left=right = diagonal before 0-diagonal)
    // base case, reached the end
    if(left > right) {
        return NULL;
    }

    // get index (for the word node array) of word node to add from the rootTable using the subarry range
    int rootValue = rootTable[left][right];
    int index = rootValue-1;

    // add the word node at the current index here
    root = newTreeNode(wordArr[index], left, right);

    // split the word node array and try to add root nodes to the left and right subtrees
    root->left = insertFromRootTable(root->left, wordArr, rootTable, left, rootValue-1);
    root->right = insertFromRootTable(root->right, wordArr, rootTable, rootValue+1, right);

    return root;
}

/**
 * Searches for a given word node key within the optimal BST tree and prints as it goes
 * @param TreeNode *root -the tree node being searched
 * @param float **mainTable -matrix (passed-by-ref)
 * @param char *key -the key (word) being searched for
 * @param int *keyFound -pass boolean by-ref as int=0: set to 1 if match is found, otherwise remains 0
 */
void searchOptimalBST(TreeNode* root, float **mainTable, char* key, int *keyFound) {
    // stop if reach a leaf's left or right NULL child -- also, climb out of recursive stack quickly if a match has been found
    if(root == NULL || *keyFound) {
        return;
    }
    // print the value the key is being compare with and the minimum average number of comparisons of the subtree
    printf("Compared with '%16s' (%.6f), ", root->wordData.word, mainTable[root->i][root->j]);

    // check if it's a match, otherwise continue checking the left and right subtrees (print found, left, or right -> show traversal)
    if(strcmp(key, root->wordData.word) == 0) {
        *keyFound = 1; //found match, set true
        printf("found.\n");
        return;
    } else if(strcmp(key, root->wordData.word) < 0) {
        printf("go left subtree.\n");
        searchOptimalBST(root->left, mainTable, key, keyFound);
    } else {
        printf("go right subtree.\n");
        searchOptimalBST(root->right, mainTable, key, keyFound);
    }
}