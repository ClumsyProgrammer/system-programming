
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "MaxHeapTree.h"


///////////////     CREATE      ///////////////////

// new empty tree
MaxTree* newMaxTree(){
    MaxTree* tree = malloc(sizeof(MaxTree));
    tree->root = NULL;
    tree->height=0;
    tree->next_available=NULL;
    printf("New tree @ %p\n",tree);
    return tree;
}

// add new node to the tree given or update an existing
MaxNode* newMaxNode(MaxTree* tree, char* number, double gain){

    if(tree->root==NULL){
        // if tree is emtpy
        MaxNode* node = (MaxNode*) malloc(sizeof(MaxNode));
        node->number = number;
        node->gain = gain;
        node->left   = NULL;
        node->right  = NULL;
        node->parent = NULL;
        tree->root = node;
        tree->next_available = node;
        //printf("New node @ %p\n",node);
        //printf("Available node @ %p\n",tree->next_available);
        return node;
    }else{
        // search to find identical node
        MaxNode* temp = searchTree(tree->root,number);
        if(temp == NULL){
            // insert new node
            MaxNode* node = (MaxNode*) malloc(sizeof(MaxNode));
            node->number = number;
            node->gain = gain;
            node->left   = NULL;
            node->right  = NULL;
            // find next available parent in order to maintain balance
            MaxNode *parent = tree->next_available;
            node->parent = parent;
            if(parent->right==NULL){
                parent->right=node;
                //printf("Available node @ %p\n",tree->next_available);
            }else{
                parent->left=node;
                tree->next_available = findAvailableParent(tree);
                //printf("Available node @ %p\n",tree->next_available);
            }
            // swap nodes if needed to maintain property of max heap
            BubbleUp(node);
            //printf("New node @ %p\n",node);
            return node;
        }else{
            // update node
            temp->gain = temp->gain + gain;
            // swap nodes if needed to maintain property of max heap
            BubbleUp(temp);
            return temp;
        }

    }

}



//////////// MAX HEAP PROPERTIES /////////////////////////

// linear search of binary tree to find subscriber
MaxNode* searchTree(MaxNode* node, char* number){
    MaxNode* temp;
    int found;
    found = strcmp(node->number,number);
    if(found==0){
        return node;
    }else{
        if(node->left!=NULL){
            temp = searchTree(node->left,number);
            if(temp!=NULL){
                return temp;
            }
        }
        if(node->right!=NULL){
            temp = searchTree(node->right,number);
            if(temp!=NULL){
                return temp;
            }else{
                return NULL;
            }
        }
        return NULL;
    }
}



// level order traversal to find next available  parent node
MaxNode* findAvailableParent(MaxTree* tree){

    MaxNode* temp = findParent(tree->root,tree->height);
    if(temp!=NULL){
        return temp;
    }else{
        tree->height++;
        temp = findParent(tree->root,tree->height);
        return temp;
    }

}


// find the most right node with null children at given height
MaxNode* findParent(MaxNode* node, int height){

    // children available
    if(node->right==NULL || node->left==NULL){
        return node;
    }
    // given level exhausted
    if(height==0){
        return NULL;
    }

    // Move further down
    MaxNode* temp;
    temp = findParent(node->right,height-1);
    if(temp!=NULL){
        return temp;
    }
    temp = findParent(node->left,height -1);
    return temp;

}

// compare and swap nodes
void BubbleUp(MaxNode* node){
    MaxNode* parent = node->parent;
    if(parent == NULL || (node->gain)<(parent->gain)){
        // if root reached or order correct
        return;
    }else{
        // swap number and gain
        char* phone;
        double charge;
	phone = node->number;
	node->number = parent->number;
	parent->number = phone;
        charge = node->gain;
        node->gain = parent->gain;
        parent->gain = charge;
        phone = NULL;
        // check parent of parent too, recursively
        BubbleUp(parent);
    }
}


/////////////    DELETE TREE    //////////////

void deleteMaxTree(MaxTree* tree){
    // delete nodes recursively
    if(tree->root!=NULL)
    deleteMaxNode(tree->root);
    free(tree);

}
void deleteMaxNode(MaxNode* node){

    if(node->right!=NULL){
        deleteMaxNode(node->right);
    }
    if(node->left!=NULL){
        deleteMaxNode(node->left);
    }
    MaxNode* parent = node->parent;
    if(parent!=NULL){
        if(parent->right!=NULL){
            parent->right=NULL;
        }else{
            parent->left = NULL;
        }
    }

    free(node);
}

/////////////    DELETE TREE AND NUMBERS   //////////////

void deleteMaxTreeN(MaxTree* tree){
    // delete nodes recursively
    if(tree->root!=NULL)
    deleteMaxNodeN(tree->root);
    free(tree);

}
void deleteMaxNodeN(MaxNode* node){

    if(node->right!=NULL){
        deleteMaxNode(node->right);
    }
    if(node->left!=NULL){
        deleteMaxNode(node->left);
    }
    MaxNode* parent = node->parent;
    if(parent!=NULL){
        if(parent->right!=NULL){
            parent->right=NULL;
        }else{
            parent->left = NULL;
        }
    }
    free(node->number);
    free(node);
}

/////////////      PRINT TREE      ///////////////


void printTree(MaxTree* tree){
    printTreeNode(tree->root);
}

void printTreeNode(MaxNode* node){
    if(node!=NULL){
        printf("Number: %s, Total cost: %f\n", node->number,node->gain);
        printTreeNode(node->right);
        printTreeNode(node->left);
    }

}

void printMaxTreeNode(MaxNode* node, double times){
    // topdest -> print all nodes that have the max property
    if(node!=NULL && node->gain>=times){
        printf("Number: %s, priority: %f\n",node->number,node->gain);
        printMaxTreeNode(node->right,times);
        printMaxTreeNode(node->left,times);
    }

}
