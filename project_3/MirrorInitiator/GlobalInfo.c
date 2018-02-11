#include "GlobalInfo.h"


// create new info
Info* newInfo(){
    Info* info = malloc(sizeof(Info));
    info->mirror_address=malloc(sizeof(LINE_MAX));
    info->list = newList();
    info->mode=1;
    info->mirror_port=0;
    return info;
}


// delete info
void deleteInfo(Info* info){
    free(info->mirror_address);
    deleteList(info->list);
    printf("Info deleted\n");
}