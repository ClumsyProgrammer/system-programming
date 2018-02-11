#include "GlobalInfo.h"

#include <limits.h>
#include <malloc.h>

// create new info
Info* newInfo(){

    Info* info = malloc(sizeof(Info));
    info->dir_of_content=malloc(sizeof(PATH_MAX));
    info->R_list = newRList();
    info->T_list = newTList();
    info->mode=1;
    info->port=0;
    info->bytes_sent=0;
    info->bytes_received=0;
    return info;
}


// delete info
void deleteInfo(Info* info){
    if(info->dir_of_content!=NULL)
    free(info->dir_of_content);
    if(info->R_list!=NULL)
    deleteRList(info->R_list);
    if(info->T_list!=NULL)
    deleteTList(info->T_list);
    printf("Info deleted\n");
}


// print info
void printInfo(Info* info){

    printf("==================================================================\n");
    printf("GLOBAL INFO\n");
    printf("------------------------------------------------------------------\n");
    printf("Local directory of files: %s\n",info->dir_of_content);
    printf("Listening to port: %d\n",info->port);
    printf("Workers available: %d\n",info->num_of_workers);
    printf("Current mode: %d\n",info->mode);
    printf("List of pending requests: \n");
    printRList(info->R_list);
    printf("List of active threads: \n");
    printTList(info->T_list);
    printf("Bytes sent: %d\n",info->bytes_sent);
    printf("Bytes received: %d\n",info->bytes_received);
    printf("==================================================================\n");

}
