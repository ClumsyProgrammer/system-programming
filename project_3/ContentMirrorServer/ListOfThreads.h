
#ifndef CONTENTMIRRORSERVER_LISTOFWORKERS_H
#define CONTENTMIRRORSERVER_LISTOFWORKERS_H

#include <pthread.h>


typedef struct ThreadNode{

    struct ThreadNode *next;
    struct ThreadNode *previous;
    pthread_t tid;


}ThreadNode;

typedef struct ListOfThreads{

    ThreadNode* head;
    ThreadNode* tail;


}ListOfThreads;

// create R_list of requests
ListOfThreads* newTList();
ThreadNode* newTNode(ListOfThreads* list, pthread_t tid);

// find Tnode
ThreadNode* findTNode(ListOfThreads* list, pthread_t tid);

// print R_list of requests
void printTList(ListOfThreads* list);
void printTNode(ThreadNode* node);

// delete R_list of requests
void deleteTList(ListOfThreads* list);
void deleteTNode(ListOfThreads* list,ThreadNode* node);





#endif //CONTENTMIRRORSERVER_LISTOFWORKERS_H
