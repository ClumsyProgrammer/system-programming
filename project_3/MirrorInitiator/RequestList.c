#include "RequestList.h"


////////       create list of records      ////////////////

ListOfRequests* newList(){

    ListOfRequests* list = malloc(sizeof(ListOfRequests));
    //printf("new list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;

    return list;


}

RequestNode* newNode(ListOfRequests* list, char* request){


    RequestNode* node = malloc(sizeof(RequestNode));
    //printf("new node @ %p\n",node);

    char* pch = strtok(request,",");

    if(list->tail==NULL){   // if list is empty -> new root
        list->head = node;
        list->tail = node;
        node->next = NULL;
        node->previous = NULL;
        node->request = malloc(sizeof(char)*(strlen(pch)+5));
        char* to_send = malloc(sizeof(char)*MAXBUFF);
        sprintf(to_send,"GET %s",request);
        strcpy(node->request,to_send);
        free(to_send);

    }else{
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
        node->next = NULL;
        node->request = malloc(sizeof(char)*(strlen(pch)+5));
        char* to_send = malloc(sizeof(char)*MAXBUFF);
        sprintf(to_send,"GET %s",request);
        strcpy(node->request,to_send);
        free(to_send);
    }
    return node;
}


//////////      print list of records      ///////////////


void printList(ListOfRequests* list){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        RequestNode* node;
        node = list->head;
        while(node!=NULL){
            printNode(node);
            node = node->next;
        }
    }
}

void printNode(RequestNode *node){
    printf("fetch: %s\n",node->request);
}



//////////        delete list and records        ////////////////


void deleteList(ListOfRequests* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        RequestNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deleteNode(list,current);
            current=next;
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
    }

    //printf("List deleted\n");
}

void deleteNode(ListOfRequests* list,RequestNode* node){

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










