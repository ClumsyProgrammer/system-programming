#ifndef MIRRORINITIATOR_REQUESTLIST_H
#define MIRRORINITIATOR_REQUESTLIST_H

#include <malloc.h>
#include <string.h>
#include <limits.h>
#define MAXBUFF 1024

typedef struct RequestNode{

    struct RequestNode *next;
    struct RequestNode *previous;
    char* request;

}RequestNode;

typedef struct ListOfRequests{

    RequestNode* head;
    RequestNode* tail;


}ListOfRequests;

// create list of requests
ListOfRequests* newList();
RequestNode* newNode(ListOfRequests* list, char* request);

// print list of requests
void printList(ListOfRequests* list);
void printNode(RequestNode* node);

// delete list of requests
void deleteList(ListOfRequests* list);
void deleteNode(ListOfRequests* list,RequestNode* node);



#endif //MIRRORINITIATOR_REQUESTLIST_H
