#ifndef GLOBAL_H
#define GLOBAL_H

#include <signal.h>
#include <malloc.h>
#include "jms_coord_client.h"
#include "jms_coord_pool_list.h"

typedef struct Info{

    char path[150];
    int pool_size;
    int mode;
    int job_sum;
    int pool_sum;
    Client* console;
    PoolList* list;

}Info;

// dilosi global metavlitis
extern Info* info;

// create new info
Info* newInfo();
// delete info
void deleteInfo(Info* info);


#endif //WERHAUZ_GLOBAL_H
