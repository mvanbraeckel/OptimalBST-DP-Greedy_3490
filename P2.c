/**
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @course CIS*3490: Analysis and Design of Computer Algorithms - A4
 * @version 23/03/2019
 * @file P2.c
 * @brief A4 Problem 2 - optimal binary search tree: greedy technique
 */

#include "a4header.h"

void p2() {
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

    // -------------------------------------------------------------------------------------------------------------

    // create the actual (optimal) BST greedily
    TreeNode *root = createGreedyBST(wordArr, numUniqueWords);

    printf("\nP2 - Optimal BST: Greedy Technique (enter a blank line to exit)");
    // infinite menu loop, only ending program with '7' as input
    while(1) {
        printf("\nEnter a key: ");
        fgets(user_input, 51, stdin);
        flushInput(user_input);

        // checks user input if they want to exit
        if(user_input[0] == '\0') {
            printf("\nGood bye! --leaving Optimal BST: Greedy Technique\n");    // display closing msg

            // free tree before leaving 
            freeTree(root);

            return;

        } else {                                  // =============================== ELSE ================================
            // search the tree
            int keyFound = 0;
            searchGreedyBST(root, user_input, &keyFound);
            if(!keyFound) {
                printf("Not found.\n");
            }
        }
    } // end menu loop
}

// ===============================================================================================

/**
 * Creates optimal BST from root table
 * @param WordNode *wordArr -array of word nodes from the file
 * @param int numUniqueWords -#of unique words from the file
 * @return the root of the greedily created (optimal) BST
 */
TreeNode* createGreedyBST(WordNode *wordArr, int numUniqueWords) {
    // create an optimal BST (the root node)
    TreeNode* root = NULL;
    // NOTE: index corresponsing to wordArr is rootValue-1 -- NOTE: numCols = numUniqueWords+1
    // start by grabbing top-right index (we start row from 1, and last column)
    root = insertGreedily(root, wordArr, 0, numUniqueWords-1); // inclusive left and right indices
    return root;
}

/**
 * Adds a word node to the (optimal) greedy BST
 * @param TreeNode *root -the subtree being looked at
 * @param WordNode *wordArr -the array of word nodes from the file
 * @param int left -inclusive left-side start of the sub array of wordArr
 * @param int right -inclusive right-side end of the sub array of wordArr
 * @return -the (new) (optimal) greedy BST
 */
TreeNode* insertGreedily(TreeNode *root, WordNode *wordArr, int left, int right) {
    // to insert greedily means to be greedy towards taking the highest frequency word's WordNode from the curent word node subarray
    // base case, reached the end
    if(left > right) {
        return NULL;
    }

    // find the highest frequency word in the word node sub array
    float max = 0.0;
    int index = left;
    for(int i = left; i <= right; i++) {
        // reset placeholders if current is higher
        if(wordArr[i].freq > max) {
            max = wordArr[i].freq;
            index = i;
        }
    }

    // add the word node at the current index here
    root = newTreeNode(wordArr[index], left, right);

    // split the word node array and try to greedily add root nodes to the left and right subtrees
    root->left = insertGreedily(root->left, wordArr, left, index-1);
    root->right = insertGreedily(root->right, wordArr, index+1, right);

    return root;
}

/**
 * Searches for a given word node key within the (optimal) greedy BST tree and prints as it goes
 * @param TreeNode *root -the tree node being searched
 * @param char *key -the key (word) being searched for
 * @param int *keyFound -pass boolean by-ref as int=0: set to 1 if match is found, otherwise remains 0
 */
void searchGreedyBST(TreeNode* root, char* key, int *keyFound) {
    // stop if reach a leaf's left or right NULL child -- also, climb out of recursive stack quickly if a match has been found
    if(root == NULL || *keyFound) {
        return;
    }
    // print the value the key is being compare with and the minimum average number of comparisons of the subtree
    printf("Compared with '%16s' (%.6f), ", root->wordData.word, root->wordData.freq);

    // check if it's a match, otherwise continue checking the left and right subtrees (print found, left, or right -> show traversal)
    if(strcmp(key, root->wordData.word) == 0) {
        *keyFound = 1; //found match, set true
        printf("found.\n");
        return;
    } else if(strcmp(key, root->wordData.word) < 0) {
        printf("go left subtree.\n");
        searchGreedyBST(root->left, key, keyFound);
    } else {
        printf("go right subtree.\n");
        searchGreedyBST(root->right, key, keyFound);
    }
}
