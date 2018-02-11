#ifndef MIRRORINITIATOR_REQUESTLIST_H
#define MIRRORINITIATOR_REQUESTLIST_H

#include "Request.h"


typedef struct RequestNode{

    struct RequestNode *next;
    struct RequestNode *previous;
    Request* request;

}RequestNode;


typedef struct ListOfRequests{

    RequestNode* head;
    RequestNode* tail;

}ListOfRequests;


// create R_list of requests
ListOfRequests* newRList();
RequestNode* newRNode(ListOfRequests* list, Request* request);

// print R_list of requests
void printRList(ListOfRequests* list);
void printRNode(RequestNode* node);

// delete R_list of requests
void deleteRList(ListOfRequests* list);
void deleteRNode(ListOfRequests* list,RequestNode* node);

// extract request
Request* extractRNode(ListOfRequests* list,RequestNode* node);


#endif //MIRRORINITIATOR_REQUESTLIST_H
