#ifndef MIRRORINITIATOR_GLOBALINFO_H
#define MIRRORINITIATOR_GLOBALINFO_H

#include "RequestList.h"
#include "ListOfThreads.h"


typedef struct Info{

    char* dir_of_content;
    int port;
    int num_of_workers;
    int mode;
    ListOfRequests* R_list;
    ListOfThreads* T_list;
    int bytes_sent;
    int bytes_received;


}Info;


// dilosi global metavlitis
extern Info* global_info;

// create new info
Info* newInfo();

// delete info
void deleteInfo(Info* info);

// print info
void printInfo(Info* info);


#endif //MIRRORINITIATOR_GLOBALINFO_H
