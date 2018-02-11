#ifndef COORDINATOR_LISTOFPOOLS_H
#define COORDINATOR_LISTOFPOOLS_H

#include <malloc.h>
#include "jms_coord_pool.h"

typedef struct PoolNode{

    struct PoolNode *next;
    struct PoolNode *previous;
    Pool* pool;

}PoolNode;

typedef struct PoolList{

    PoolNode* head;
    PoolNode* tail;

}PoolList;

// create list of pools
PoolList* newPoolList();
PoolNode* newPoolNode(PoolList* list, Pool* pool);

// print list of pools
void printPoolList(PoolList* list);
void printPoolNode(PoolNode* node);

// delete list of pools
void deletePoolList(PoolList* list);
void deletePoolNode(PoolList* list,PoolNode* node);


#endif //COORDINATOR_LISTOFPOOLS_H
