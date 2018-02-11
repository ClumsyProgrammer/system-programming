#include "HashTable.h"

#ifndef WERHAUZ_RECORDMANAGEMENT_H
#define WERHAUZ_RECORDMANAGEMENT_H
// insert record to structures
int insertRecordtoHash(HashTable* hash, Record* record, int bucket);
void insertRecordtoTree(Record* record);
// remove record from hash table 1
int removeRecordfromHash(HashTable* hash, char* hash_field, char* id);
// find record in hash table
ListOfRecords* findRecordsinHash(HashTable* hash, char* hash_field, int date1, int date2, char* time1, char* time2);
int checkDate(int thres1, int thres2, int value);
int checkTime(const char* thres01, const char* thres02, const char* valueee, int date);
#endif //WERHAUZ_RECORDMANAGEMENT_H
