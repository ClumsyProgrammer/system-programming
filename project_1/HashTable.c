#define _GNU_SOURCE
#include "HashTable.h"
#include <stdlib.h>


/////////          create hash table     //////////////////////

HashTable* newHashTable(int num_of_buckets, int size_of_bucket, int field){

    // malloc gia domi hash syn to megethos tou pinaka
    HashTable* hash = malloc(sizeof(HashTable)+num_of_buckets*sizeof(Bucket*));

    hash->field = field;
    hash->numberOfBuckets=num_of_buckets;

    int i=0;
    for(i=0;i<num_of_buckets;i++){

        // gia kathe thesi tou pinaka ftiaxnetai enas kainourios bucket
        hash->table[i]=newBucket(size_of_bucket);

    }
    printf("New hash table @ %p\n",hash);
    return hash;
}



Bucket* newBucket(int size_of_bucket){

    // ypologismos ton max entries symfona me to size pou dothike
    int size = (size_of_bucket/sizeof(Entry*));
    // malloc gia bucket syn to epithimito megethos tou pinaka
    Bucket* bucket = malloc(sizeof(Bucket)+size*sizeof(Entry*));
    //printf("new bucket @ %p\n",bucket);
    bucket->size=size;
    bucket->entries=0;
    bucket->next=NULL;
    int i;
    for(i=0;i<size;i++){

        // gia kathe thesi tou pinaka ftiaxnetai mia nea entry
        bucket->index[i]=newEntry();
    }
    return bucket;
}




Entry* newEntry(){

    // malloc gia mia entry
    Entry* entry = malloc(sizeof (Entry));
    //printf("new entry @ %p\n",entry);

    // i lista den arxikopoieitai
    entry->phone = NULL;
    entry->records=NULL;

    return entry;
}


/////////         delete hash table and lists of records     //////////////////////


void freeHashTable(HashTable* hash){

    int i=0;
    for(i=0;i<hash->numberOfBuckets;i++){
        // gia kathe thesi tou pinaka eleutherose ton antistoixo bucket
        freeBucket(hash->table[i]);

    }

    // delete hash
    free(hash);
    printf("Hash table deleted\n");

}



void freeBucket(Bucket* bucket){

    if(bucket->next!=NULL){
        //  an yparxei overflow bucket, eleutherose ton
        freeBucket(bucket->next);
    }

    int i;
    for(i=0;i<bucket->size;i++){
        // gia kathe thesi tou pinaka eleutherose tin antistoixi entry
        freeEntry(bucket->index[i]);
    }

    // delete bucket
    free(bucket);

}




void freeEntry(Entry* entry){

    // ean yparxei lista, diagrapse ti
    if(entry->records!=NULL){
        deleteList(entry->records);
    }
    // delete entry
    free(entry);

}



/////////         print hash table     //////////////////////



void printHashTable(HashTable* hash){

    printf("Hash table by field %d with %d buckets\n",hash->field,hash->numberOfBuckets);
    printf("-----------------------------------------------------\n");

    int i=0;
    for(i=0;i<hash->numberOfBuckets;i++){
        printf("Bucket %d\n",i);
        // gia kathe thesi tou pinaka ektypose ton antistoixo bucket

        printBucket(hash->table[i]);
        printf("-----------------------------------------------------\n");


    }



}



void printBucket(Bucket* bucket){

    int i;
    for(i=0;i<bucket->size;i++){
        printf("Entry %d\n",i);
        // gia kathe thesi tou pinaka ektypose tin antistoixi entry

        printEntry(bucket->index[i]);
        printf("*****************************\n");
    }
    if(bucket->next!=NULL){
        printf("Going to overflow\n");
        // ean yparxei overflow, ektypose ton
        printBucket(bucket->next);
    }


}




void printEntry(Entry* entry){

    // ean yparxei lista eggrafon ektypose ti
    if(entry->records!=NULL){
        printList(entry->records);
    }else{
        //printf("Entry empty\n");
    }


}


///////////////      delete hash table only      ////////////////////////

void deleteHashOnly(HashTable* hash){
    int i=0;
    for(i=0;i<hash->numberOfBuckets;i++){
        // gia kathe thesi tou pinaka delete ton antistoixo bucket
        deleteBucket(hash->table[i]);

    }

    free(hash);
    printf("Hash table deleted\n");
}
void deleteBucket(Bucket* bucket){
    //printf("bucket: %p\n",bucket);
    if(bucket->next!=NULL){
        // ean yparxei overflow, diagrapse ton
        deleteBucket(bucket->next);
    }

    int i;
    for(i=0;i<bucket->size;i++){
        // gia kathe thesi tou pinaka diegrapse tin antistoixi entry
        deleteEntry(bucket->index[i]);
    }

    // eleutherose ton bucket
    free(bucket);
}
void deleteEntry(Entry* entry){
    // eleutherose tin entry
    // i lista ton eggrafon den diagrafetai
    free(entry);
}



////////////////      dump hash table to file         ///////////////////

///////  -> to arxeio mporei na ksanadothei os eisodos

void printHashtoFile(HashTable* hash, char* filename){

    FILE *stream;
    // open file and overwrite or create new
    stream=fopen(filename,"w+");
    int i=0;
    for(i=0;i<hash->numberOfBuckets;i++){
        char * out = NULL;
        asprintf(&out,"Bucket %d\n",i);
        fputs(out,stream);
        free(out);
        //printf("Bucket %d\n",i);
        // gia kathe thesi tou pinaka ektypose ton antistoixo bucket sto stream
        printBuckettoFile(hash->table[i],stream);
        //printf("-----------------------------------------------------\n");


    }
    fclose(stream);
}
void printBuckettoFile(Bucket* bucket, FILE *stream){
    int i;
    for(i=0;i<bucket->size;i++){
        char *out;
        asprintf(&out,"Entry %d\n",i);
        fputs(out,stream);
        free(out);
        //printf("Entry %d\n",i);
        // gia kathe thesi tou pinaka ektypose tin antistoixi entry sto stream
        printEntrytoFile(bucket->index[i],stream);
        //printf("*****************************\n");
    }
    if(bucket->next!=NULL){
        //printf("Going to overflow\n");
        // an yparxei overflow na graftei kai autos sto stream
        printBuckettoFile(bucket->next,stream);
    }
}
void printEntrytoFile(Entry* entry, FILE *stream){
    if(entry->records!=NULL){
        // an ektypothei i lista sto stream
        printListtoFile(entry->records,stream);
    }else{
        //printf("Entry empty\n");
    }


}

