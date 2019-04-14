/**
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @course CIS*3490: Analysis and Design of Computer Algorithms - A4
 * @version 23/03/2019
 * @file a4header.h
 * @brief header file for A4
 */

#ifndef A4HEADER_H
#define A4HEADER_H

// ============================== INCLUDES ==============================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>

// ============================== STRUCTS ===============================
typedef struct word_count_node {
    int count;
    float freq;
    char word[51];  // max 50 chars
} WordNode;

typedef struct bst_tree_struct {
    struct bst_tree_struct *left;
    struct bst_tree_struct *right;
    WordNode wordData;
    int i;
    int j;
} TreeNode;

// ======================================================================
void p1();
void p2();

// ======================================================================

void flushInput(char *input);


int readStrings(char* filename, char* arr[2045]);
int wordNodeCompar(const void *a, const void *b);

TreeNode* newTreeNode(WordNode toAdd, int i, int j);
void freeTree(TreeNode *root);


void constructTables(float **mainTable, int **rootTable, WordNode *wordArr, int numUniqueWords);
TreeNode* createOptimalBST(WordNode *wordArr, int **rootTable, int numUniqueWords);
TreeNode* insertFromRootTable(TreeNode *root, WordNode *wordArr, int **rootTable, int left, int right);
void searchOptimalBST(TreeNode* root, float **mainTable, char* key, int *keyFound);

TreeNode* createGreedyBST(WordNode *wordArr, int numUniqueWords);
TreeNode* insertGreedily(TreeNode *root, WordNode *wordArr, int left, int right);
void searchGreedyBST(TreeNode* root, char* key, int *keyFound);

#endif