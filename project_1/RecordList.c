#include <malloc.h>
#include "RecordList.h"

////////       create list of records      ////////////////

ListOfRecords* newList(){

    ListOfRecords* list = malloc(sizeof(ListOfRecords));
    //printf("new list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;

    return list;


}

ListNode* newNode(ListOfRecords* list, Record* record){

    ListNode* node = malloc(sizeof(ListNode));
    //printf("new node @ %p\n",node);

    if(list->tail==NULL){   // if list is empty -> new root
        list->head = node;
        list->tail = node;
        node->next = NULL;
        node->previous = NULL;
        node->record = record;
    }else{
        list->tail->next = node;
        node->previous = list->tail;
        list->tail = node;
        node->next = NULL;
        node->record = record;
    }
    return node;
}


//////////      print list of records      ///////////////


void printList(ListOfRecords* list){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        ListNode* node;
        node = list->head;
        while(node!=NULL){
            printNode(node);
            node = node->next;
        }
    }
}

void printNode(ListNode *node){
    printRecord(node->record);
}



//////////        delete list and records        ////////////////


void deleteList(ListOfRecords* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        ListNode *current, *next;
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

void deleteNode(ListOfRecords* list,ListNode* node){

    if(node->next==NULL){  // delete tail
      list->tail = node->previous;
    }

    if(node->previous==NULL){ // delete head
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{
        ListNode* previous = node->previous;
        ListNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }

    deleteRecord(node->record);
    free(node);
}



///////////       delete list only         ////////////


void freeList(ListOfRecords* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        ListNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            freeNode(list,current);
            current=next;
        }
		    list->head = NULL;
    	  list->tail = NULL;
        free(list);
    }

    //printf("List deleted\n");
}


void freeNode(ListOfRecords* list,ListNode* node){

  if(node->next==NULL){  // delete tail
    list->tail = node->previous;
  }

    if(node->previous==NULL){
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{
        ListNode* previous = node->previous;
        ListNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }
    node->previous=NULL;
	  node->next=NULL;
	  node->record=NULL;
    free(node);
}



////////       dump list to file       /////////////////


void printListtoFile(ListOfRecords* list,FILE *stream){
    if(list->tail==NULL){
        //printf("List empty\n");
    }else{
        ListNode* node;
        node = list->head;
        while(node!=NULL){
            printNodetoFile(node,stream);
            node = node->next;
        }
    }
}
void printNodetoFile(ListNode* node, FILE *stream){

    char* test = printRecordtoFile(node->record);
    fputs(test,stream);
    //printf("%s",test);
    free(test);
}
