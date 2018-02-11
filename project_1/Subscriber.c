//
// Created by katerina on 16/3/2017.
//

#include <malloc.h>
#include <string.h>
#include "Subscriber.h"


////   CREATE NEW LIST    ////////////////////

// new empty list
ListOfSubs* newSubList(){
    ListOfSubs* list = malloc(sizeof(ListOfSubs));
    //printf("new subscriber pair list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;

    return list;
}


// add copy of callers or callees to list
// flag -> 1 for caller, 0 for callee
void addToListFromList(ListOfSubs* Plist,ListOfRecords* Rlist,int flag){

    if(Rlist->tail==NULL){  // Record list empty
        //printf("List empty\n");
    }else{
        ListNode* node;
        node = Rlist->head;
        while(node!=NULL){

          // add node to list
            if(flag==0){
                newSubNode(Plist,node->record->originator_number);

            }else if(flag==1){
                newSubNode(Plist,node->record->destination_number);

            }
            node = node->next;
        }
    }

}


////    create new node
SubNode* newSubNode(ListOfSubs* list, char* caller){

  // if same number already exists ignore
    int i = containsSub(list,caller);
    if(i==0){

        SubNode* node = malloc(sizeof(SubNode));
        node->subscriber = malloc(sizeof(char)*(strlen(caller)+1));
        strcpy(node->subscriber,caller);
        //printf("new subscriber node @ %p\n",node);
        if(list->tail==NULL){
            list->head = node;
            list->tail = node;
            node->next = NULL;
            node->previous = NULL;
        }else{
            list->tail->next = node;
            node->previous = list->tail;
            list->tail = node;
            node->next = NULL;
        }
        return node;
    }
    return NULL;
}


////////    PRINT list     ///////////


void printSubList(ListOfSubs* list){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        SubNode* node;
        node = list->head;
        while(node!=NULL){
            printSubNode(node);
            node = node->next;
        }
    }
}
void printSubNode(SubNode* node){
    printf("Subscriber: %s\n",node->subscriber);
}


///////////   delete list   ////////////////


void deleteSubList(ListOfSubs* list){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        SubNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deleteSubNode(list,current);
            current=next;
        }

        free(list);
    }

    //printf("List deleted\n");
}
void deleteSubNode(ListOfSubs* list,SubNode* node){
    if(node->previous==NULL){
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }

    }else{
        SubNode* previous = node->previous;
        SubNode* next = node->next;
        if(next!=NULL){
          next->previous=previous;
        }else{
          list->tail = node->previous;
        }

        previous->next=next;
    }

    free(node->subscriber);
    free(node);
}



//////    check if list contains subscriber's id

int containsSub(ListOfSubs* list, char* sub){
    if(list->tail==NULL){
        return 0;
    }else{
        SubNode* node;
        node = list->head;
        while(node!=NULL){
            int i= strcmp(node->subscriber,sub);
            if(i==0){
                return -1;
            }
            node = node->next;
        }
    }
    return 0;
}


/////      return list of nodes that exist in both lists

ListOfSubs* SubListIntersection(ListOfSubs* subs1, ListOfSubs* subs2){


    if(subs1==NULL || subs2==NULL || subs1->tail==NULL || subs2->tail==NULL){
      // if any of the lists is empty reurn null
        return NULL;
    }else{
        ListOfSubs* new = newSubList();
        SubNode* node;
        node = subs1->head;
        while(node!=NULL){
            int i= containsSub(subs2,node->subscriber);
            if(i==-1){
                newSubNode(new,node->subscriber);
            }
            node = node->next;
        }
        return new;
    }




}
