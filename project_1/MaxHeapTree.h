

#ifndef WERHAUZ_MAXHEAPTREE_H
#define WERHAUZ_MAXHEAPTREE_H

typedef struct MaxNode{

    char* number;
    double gain;
    struct MaxNode* right;
    struct MaxNode* left;
    struct MaxNode* parent;

}MaxNode;

typedef struct MaxTree{

    MaxNode *root;
    int height;
    MaxNode* next_available;

}MaxTree;

// create tree
MaxTree* newMaxTree();
MaxNode* newMaxNode(MaxTree* tree, char* number, double gain);

// max heap tree properties
MaxNode* searchTree(MaxNode* node, char* number);
MaxNode* findAvailableParent(MaxTree* tree);
MaxNode* findParent(MaxNode* node, int height);
void BubbleUp(MaxNode* node);

// delete max heap tree
void deleteMaxTree(MaxTree* tree);
void deleteMaxNode(MaxNode* node);

// delete max heap tree and numbers
void deleteMaxTreeN(MaxTree* tree);
void deleteMaxNodeN(MaxNode* node);

// print tree
void printTree(MaxTree* tree);
void printTreeNode(MaxNode* node);

// print top callers
void printMaxTreeNode(MaxNode* node, double times);

#endif //WERHAUZ_MAXHEAPTREE_H




