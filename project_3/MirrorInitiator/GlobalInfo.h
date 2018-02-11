#ifndef MIRRORINITIATOR_GLOBALINFO_H
#define MIRRORINITIATOR_GLOBALINFO_H

#include "RequestList.h"
#include <limits.h>

typedef struct Info{

    char* mirror_address;
    int mirror_port;
    int mode;
    ListOfRequests* list;

}Info;


// dilosi global metavlitis
extern Info* global_info;

// create new info
Info* newInfo();
// delete info
void deleteInfo(Info* info);


#endif //MIRRORINITIATOR_GLOBALINFO_H
