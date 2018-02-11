#include "RecordManagement.h"
#include "HashTable.h"
#include "CallRecord.h"
#include "Global.h"
#include "HashFunction.h"
#include "RecordList.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>

// insert record to hash table
int insertRecordtoHash(HashTable* hash, Record* record, int bucket){
    Bucket* buck = hash->table[bucket];
    // search bucket and its overflow buckets for identical entry
    while(buck->next!=NULL){
        Entry* entry;
        int comp;
        ////    check if there is entry with the same hash field
        int i;
        for(i=0;i<buck->entries;i++){
            entry = buck->index[i];
            // compare hash field of table
            if(hash->field==0){
                comp = strcmp(record->originator_number,entry->phone);
            }else{
                comp = strcmp(record->destination_number,entry->phone);
            }
            // add record to entry's  list
            if(comp==0){
                if(entry->records==NULL)
                    entry->records = newList();
                newNode(entry->records,record);
                //printList(entry->records);
                return 0;
            }
        }
        buck = buck->next;
    }
    //search last overflow bucket
    if(buck->entries<buck->size){

      // if there is still space
        Entry* entry;
        int comp;
        ////    check if there is entry with the same hash field
        int i;
        for(i=0;i<buck->entries;i++){
            entry = buck->index[i];
            if(hash->field==0){
                comp = strcmp(record->originator_number,entry->phone);
            }else{
                comp = strcmp(record->destination_number,entry->phone);

            }
            if(comp==0){
                if(entry->records==NULL)
                    entry->records = newList();
                newNode(entry->records,record);
                //printList(entry->records);
                return 0;
            }
        }



        ////    if there is no entry with the same hash field add a new one
        entry = buck->index[i];
        if(hash->field==0){

            entry->phone=record->originator_number;
            entry->records = newList();
            newNode(entry->records,record);
            //printList(entry->records);
        }else{

            entry->phone=record->destination_number;
            entry->records = newList();
            newNode(entry->records,record);
            //printList(entry->records);
        }
        buck->entries++;

    }else{

      // if last bucket is full
      Entry* entry;
      int comp;
      ////    check if there is entry with the same hash field
      int i;
      for(i=0;i<buck->entries;i++){
          entry = buck->index[i];
          if(hash->field==0){
              comp = strcmp(record->originator_number,entry->phone);
          }else{
              comp = strcmp(record->destination_number,entry->phone);

          }
          if(comp==0){
              if(entry->records==NULL)
                  entry->records = newList();
              newNode(entry->records,record);
              //printList(entry->records);
              return 0;
          }
      }


      // if no entry is found, allocate a new bucket

        if(buck->next==NULL){
            //printf("No overflow bucket available\n");
            buck->next=newBucket(buck->size*sizeof(Entry*));
            buck=buck->next;
            entry = buck->index[buck->entries];
            if(hash->field==0){

                entry->phone=record->originator_number;
                entry->records = newList();
                newNode(entry->records,record);
                //printList(entry->records);
            }else{

                entry->phone=record->destination_number;
                entry->records = newList();
                newNode(entry->records,record);
                //printList(entry->records);
            }
            buck->entries++;

        }else{
            //printf("Going to overflow bucket available\n");
        }
    }

    return 0;
}



//  insert record to max heap tree

void insertRecordtoTree(Record* record){

    double cost=0;
    // calculate cost
    // if fault condition apropriate add to tree
    if(record->fault_condition>199 && record->fault_condition<300){
        if(record->type==0){
            cost = info->price_list[0];
        }else if(record->type==1){
            if(record->tarrif==1){
                cost = record->duration * info->price_list[1];
            }else if(record->tarrif==2){
                cost = record->duration * info->price_list[2];
            }
        }else if(record->type==2){
            if(record->tarrif==1){
                cost = record->duration * info->price_list[3];
            }else if(record->tarrif==2){
                cost = record->duration * info->price_list[4];
            }
        }

    //printf("cost = %f\n",cost);

    // add to max tree and update info
    info->earnings = info->earnings + cost;
    newMaxNode(info->tree,record->originator_number,cost);
    //printTree(info->tree);
  }
}




// remove reference of record from caller hashtable
int removeRecordfromHash(HashTable* hash, char* hash_field, char* id){

// go to bucket indicated by hash function
    int h1=hashFunction(info->caller->numberOfBuckets,hash_field);
    Bucket* buck = hash->table[h1];
    while(buck!=NULL){
        Entry* entry;
        int comp;
        ////    check if there is entry with the same hash field
        int i;
        for(i=0;i<buck->entries;i++){
            entry = buck->index[i];
            //printf("Looking for: %s, found: %s\n",hash_field,entry->phone);

            comp = strcmp(hash_field,entry->phone);

            if(comp==0){
                if(entry->records==NULL){
                    return 1; // No such record found
                }else{
                    ListNode* node;
                    node = entry->records->head;
                    if(node==NULL)
                        return 1;  // No such record found
                    while(node!=NULL){
                        //printf("Looking for: %s, found: %s\n",id,node->record->cdr_uniq_id);
                        comp = strcmp(id,node->record->cdr_uniq_id);
                        if(comp==0){

                            freeNode(entry->records,node);
                            return 0;  // Record found and deleted
                        }
                        node = node->next;
                    }
                    return 1;  // No such record found
                }
            }

        }

        buck = buck->next;
    }

    return 1;  // No such record found

}



// search for record in given hash table within limits of date and time
ListOfRecords* findRecordsinHash(HashTable* hash, char* hash_field, int date1, int date2, char* time1, char* time2){
    ListOfRecords* temp = newList();

    int comp, h, not_found;
    h=hashFunction(hash->numberOfBuckets,hash_field);
    not_found=0;
    Bucket* buck = hash->table[h];
    while(buck!=NULL && not_found==0){
        Entry* entry;

        ////    check if there is entry with the same hash field
        int i;
        for(i=0;i<buck->entries;i++){
            entry = buck->index[i];
            //printf("Looking for: %s, found: %s\n",hash_field,entry->phone);

            comp = strcmp(hash_field,entry->phone);

            if(comp==0){
                not_found=1;
                if(entry->records==NULL){
                    return NULL; // Empty list
                }else{
                    if(time1==NULL && date1==0){     // if no dates or times are given
                        deleteList(temp);           // return entry's whole list
                        return entry->records;
                    }else{
                        ListNode* node;
                        node = entry->records->head;
                        if(node==NULL)
                            return NULL;  // Empty list
                        while(node!=NULL){
                            int result;
                            //printf("Looking for date: from: %d to: %d, found: %d \n",date1,date2,node->record->date);
                            //printf("Looking for time: from: %s to: %s, found: %s\n",time1,time2,node->record->init_time);

                            // check date limits
                            int result1 = checkDate(date1,date2,node->record->date);
                            if(result1==-1){
                                result=-1;
                            }else if(result1==0){
                                result=0;
                            }else {
                                // if needed check time limits
                                result = checkTime(time1, time2, node->record->init_time, result1);
                            }


                            if(result == 0){
                                //printf("Matching record found\n");
                                newNode(temp,node->record);
                            }

                            node = node->next;
                        }
                    }

                }
                break;
            }

        }

        buck = buck->next;
    }


    return temp;
}


int checkTime(const char* thres01, const char* thres02, const char* valueee, int date){

    if(thres01==NULL){
        return  0;  // time irrelevant
    }

    int result=-1;

    char* thres1 = malloc(sizeof(thres01));
    strcpy(thres1,thres01);
    char* thres2 = malloc(sizeof(thres02));
    strcpy(thres2,thres02);
    char* value = malloc(sizeof(valueee));
    strcpy(value,valueee);

    int thres1_hour, thres2_hour, value_hour;
    int thres1_minute, thres2_minute, value_minute;

    char *token;
    token = strtok(thres1, ":");
    thres1_hour = atoi(token);
    token = strtok(NULL, ":");
    thres1_minute = atoi(token);
    token = strtok(thres2, ":");
    thres2_hour = atoi(token);
    token = strtok(NULL, ":");
    thres2_minute = atoi(token);
    token = strtok(value, ":");
    value_hour = atoi(token);
    token = strtok(NULL, ":");
    value_minute = atoi(token);


    int time1 = thres1_hour*100 + thres1_minute;
    int time2 = thres2_hour*100 + thres2_minute;
    int time = value_hour*100 + value_minute;

// check time interval according to code returned from date check
    if(date==11){
        if(time1<=time)
            result=0;
    }else if(date==12){
        if(time2>=time)
            result=0;
    }else if(date==21){
        if(time1>=time)
            result=0;
    }else if(date==22){
        if(time2<=time)
            result=0;
    }else if(date==1212){
        if(time2>=time && time1<=time)
            result=0;
        if(time1>=time && time2<=time)
            result=0;
    }

    free(thres1);
    free(thres2);
    free(value);
    return result;

}


int checkDate(int threshold1, int threshold2, int date_value){

    if(threshold1==0){
        return 1212;  // date irrelevant, check time
    }


    int result=-1;  // ektos diastimatos

    int thres1_day, thres1_month, thres1_year;
    int thres2_day, thres2_month, thres2_year;
    int value_day, value_month, value_year;

    thres1_day = threshold1/1000000;
    thres1_month= (threshold1 % 1000000)/10000;
    thres1_year = threshold1 % 10000;
    int date1 = thres1_year*10000 + thres1_month*100 + thres1_day;

    thres2_day = threshold2/1000000;
    thres2_month= (threshold2 % 1000000)/10000;
    thres2_year = threshold2 % 10000;
    int date2 = thres2_year*10000 + thres2_month*100 + thres2_day;

    value_day = date_value/1000000;
    value_month= (date_value % 1000000)/10000;
    value_year = date_value % 10000;
    int date = value_year*10000 + value_month*100 + value_day;

    if(date1<date && date<date2){
        result=0;//ok
    }else if(date1>date && date>date2){
        result=0;//ok
    }else{

        if(date1>date2){

            if(date == date1){
                result=21; // date2 is smaller, date equals date1
            }else if( date == date2){
                result=22; // date2 is smaller, date equals date2
            }


        }else if(date1<date2){

            if(date == date1){
                result=11; // date1 is smaller, date equals date1
            }else if( date == date2){
                result=12; // date1 is smaller, date equals date2
            }


        }else if(date1==date2){

            if(date == date1){
                result=1212; // date exact, check time
            }
        }
    }

    return result;
}
