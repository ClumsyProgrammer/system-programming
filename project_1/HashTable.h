#ifndef HASH_TABLE
#define HASH_TABLE
#include <stdio.h>
#include "RecordList.h"


typedef struct Entry{

    char* phone;
    ListOfRecords* records;

}Entry;

typedef struct Bucket{

    int  size;
    int  entries;
    struct Bucket* next;
    Entry* index[];

}Bucket;

typedef struct HashTable{

    int field;   // 0 for caller, 1 for callee
    int  numberOfBuckets;
    Bucket* table[];

}HashTable;

// create hash table
HashTable* newHashTable(int num_of_buckets, int size_of_bucket, int field);
Bucket* newBucket(int size_of_bucket);
Entry* newEntry();

// delete hash table and lists of records
void freeHashTable(HashTable* hash);
void freeBucket(Bucket* bucket);
void freeEntry(Entry* entry);

// print hash table
void printHashTable(HashTable* hash);
void printBucket(Bucket* bucket);
void printEntry(Entry* entry);

// delete hash table only
void deleteHashOnly(HashTable* hash);
void deleteBucket(Bucket* bucket);
void deleteEntry(Entry* entry);

// dump hash table to file
void printHashtoFile(HashTable* hash, char* filename);
void printBuckettoFile(Bucket* bucket, FILE *stream);
void printEntrytoFile(Entry* entry, FILE *stream);


#endif //HASH_TABLE
