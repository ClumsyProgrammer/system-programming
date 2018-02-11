#include "jms_coord_info.h"

// create new info
Info* newInfo(){
    Info* info = malloc(sizeof(Info));
    info->console = newClient();
    info->list = newPoolList();
    info->pool_size = 5; // default
    info->mode = 1; // running
    info->job_sum=0; // no jobs served
    info->pool_sum=0; // no pools running
    return info;
}


// delete info
void deleteInfo(Info* info){
    deletePoolList(info->list);
    deleteClient(info->console);
    free(info);
}
