
#include "ListOfThreads.h"

#include <malloc.h>


////////       create list of records      ////////////////

ListOfThreads* newTList(){

    ListOfThreads* list = malloc(sizeof(ListOfThreads));
    //printf("new R_list @ %p\n",R_list);
    list->head = NULL;
    list->tail = NULL;

    return list;


}

ThreadNode* newTNode(ListOfThreads* list, pthread_t tid){


    ThreadNode* node = malloc(sizeof(ThreadNode));
    //printf("new node @ %p\n",node);


    if(list->tail==NULL){   // if R_list is empty -> new root
        list->head = node;
        list->tail = node;
        node->next = NULL;
        node->previous = NULL;
        node->tid = tid;
    }else{
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
        node->next = NULL;
        node->tid = tid;
    }
    return node;
}




// find Tnode
ThreadNode* findTNode(ListOfThreads* list, pthread_t tid){

    if(list->tail==NULL){
        printf("List empty\n");
        return NULL;
    }else{
        ThreadNode* node;
        node = list->head;
        while(node!=NULL){
            if(node->tid == tid){
                return node;
            }
            node = node->next;
        }
    }

    return NULL;

}





//////////      print list of records      ///////////////


void printTList(ListOfThreads* list){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        ThreadNode* node;
        node = list->head;
        while(node!=NULL){
            printTNode(node);
            node = node->next;
        }
    }
}

void printTNode(ThreadNode *node){
    printf("Thread tid: %lu\n",node->tid);
}



//////////        delete list and records        ////////////////


void deleteTList(ListOfThreads* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        ThreadNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deleteTNode(list,current);
            current=next;
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
    }

    //printf("List deleted\n");
}

void deleteTNode(ListOfThreads* list,ThreadNode* node){

    if(node->next==NULL){  // delete tail
        list->tail = node->previous;
    }

    if(node->previous==NULL){ // delete head
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{
        ThreadNode* previous = node->previous;
        ThreadNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }


    free(node);
}
