#include "RequestList.h"

#include <malloc.h>

////////       create list of records      ////////////////

ListOfRequests* newRList(){

    ListOfRequests* list = malloc(sizeof(ListOfRequests));
    //printf("new R_list @ %p\n",R_list);
    list->head = NULL;
    list->tail = NULL;

    return list;

}


RequestNode* newRNode(ListOfRequests* list, Request* request){

    RequestNode* node = malloc(sizeof(RequestNode));
    //printf("new node @ %p\n",node);

    if(list->tail==NULL){   // if R_list is empty -> new root
        list->head = node;
        list->tail = node;
        node->next = NULL;
        node->previous = NULL;
        node->request = request;
    }else{
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
        node->next = NULL;
        node->request = request;
    }

    return node;

}



//////////      print list of records      ///////////////


void printRList(ListOfRequests* list){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        RequestNode* node;
        node = list->head;
        while(node!=NULL){
            printRNode(node);
            node = node->next;
        }
    }
}

void printRNode(RequestNode *node){
    printf("REQUEST: ");
    printRequest(node->request);
}



//////////        delete list and records        ////////////////


void deleteRList(ListOfRequests* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        RequestNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deleteRNode(list,current);
            current=next;
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
    }

    //printf("List deleted\n");
}

void deleteRNode(ListOfRequests* list,RequestNode* node){

    if(node->next==NULL){  // delete tail
        list->tail = node->previous;
    }

    if(node->previous==NULL){ // delete head
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{
        RequestNode* previous = node->previous;
        RequestNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }

    free(node->request);
    free(node);
}




// extract request
Request* extractRNode(ListOfRequests* list,RequestNode* node){

    if(list!=NULL && node!=NULL){

        if(node->next==NULL){  // delete tail
            list->tail = node->previous;
        }

        if(node->previous==NULL){ // delete head
            list->head = node->next;
            if(node->next!=NULL){
                node->next->previous=NULL;
            }
        }else{
            RequestNode* previous = node->previous;
            RequestNode* next = node->next;
            if(next!=NULL)
                next->previous=previous;
            previous->next=next;
        }

        Request* request= node->request;
        node->request=NULL;
        free(node);
        return request;

    }else{
        return NULL;
    }

    return NULL;

}
