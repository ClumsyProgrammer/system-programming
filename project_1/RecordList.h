#ifndef RECORD_LIST
#define RECORD_LIST

#include "CallRecord.h"
#include <stdio.h>

typedef struct ListNode{

    struct ListNode *next;
    struct ListNode *previous;
    Record* record;

}ListNode;

typedef struct ListOfRecords{

    ListNode* head;
    ListNode* tail;


}ListOfRecords;

// create list of records
ListOfRecords* newList();
ListNode* newNode(ListOfRecords* list, Record* record);

// print list of records
void printList(ListOfRecords* list);
void printNode(ListNode* node);

// delete list of records with and without deleting actual records
void deleteList(ListOfRecords* list);
void freeList(ListOfRecords* list);
void deleteNode(ListOfRecords* list,ListNode* node);
void freeNode(ListOfRecords* list,ListNode* node);

// dump list to file
void printListtoFile(ListOfRecords* list,FILE *stream);
void printNodetoFile(ListNode* node, FILE *stream);


#endif //RECORD_LIST
