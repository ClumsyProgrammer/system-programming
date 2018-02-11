//
// Created by katerina on 16/3/2017.
//

#ifndef WERHAUZ_SUBSCRIBERPAIR_H
#define WERHAUZ_SUBSCRIBERPAIR_H

#include "RecordList.h"



typedef struct SubNode{

    struct SubNode *next;
    struct SubNode *previous;
    char* subscriber;

}SubNode;

typedef struct ListOfSubs{

    SubNode* head;
    SubNode* tail;


}ListOfSubs;


// new classic list
ListOfSubs* newSubList();
SubNode* newSubNode(ListOfSubs* list, char* caller);

// add to new list or append to existing the content of list of records
void addToListFromList(ListOfSubs* Plist,ListOfRecords* Rlist,int flag);

// print list
void printSubList(ListOfSubs* list);
void printSubNode(SubNode* node);

// delete list
void deleteSubList(ListOfSubs* list);
void deleteSubNode(ListOfSubs* list,SubNode* node);

// indist special
int containsSub(ListOfSubs* list, char* sub);
ListOfSubs* SubListIntersection(ListOfSubs* subs1, ListOfSubs* subs2);

#endif //WERHAUZ_SUBSCRIBERPAIR_H
