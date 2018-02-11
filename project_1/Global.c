#include <malloc.h>
#include "Global.h"

// create new info
Info* newInfo(double price_list[5],HashTable* caller,HashTable* callee,MaxTree* tree){
    Info* info = malloc(sizeof(Info));
    info->callee = callee;
    info->caller = caller;
    info->tree = tree;
    info->earnings=0;
    int i;
    for(i=0;i<5;i++){
        info->price_list[i]=price_list[i];
    }
    return info;
}


// delete info
void deleteInfo(Info* info){
    free(info);
    printf("Info deleted\n");
}

