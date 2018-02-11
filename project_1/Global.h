
#ifndef WERHAUZ_GLOBAL_H
#define WERHAUZ_GLOBAL_H


#include "HashTable.h"
#include "MaxHeapTree.h"

typedef struct Info{

    double price_list[5];
    HashTable* caller;
    HashTable* callee;
    MaxTree* tree;
    double earnings;

}Info;

// dilosi global metavlitis
extern Info* info;
// create new info
Info* newInfo(double price_list[5],HashTable* caller,HashTable* callee,MaxTree* tree);
// delete info
void deleteInfo(Info* info);


#endif //WERHAUZ_GLOBAL_H
