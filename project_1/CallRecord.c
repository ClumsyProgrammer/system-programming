#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "CallRecord.h"

// create new empty record
Record* newEmpty() {

  Record* record = (Record *) malloc(sizeof (Record));

  return record;
}

// print record to stdout
void printRecord(Record *record) {
    printf("cdr_uniq_id: %s, originator_number: %s, destination_number: %s, date: %8d, init_time: %5s, duration: %2d, type: %d, tarrif: %d, fault_condition: %d\n",
            record->cdr_uniq_id, record->originator_number, record->destination_number,
            record->date, record->init_time, record->duration, record->type, record->tarrif,
            record->fault_condition );
}

// print record to string in file format -> for dump operation
char* printRecordtoFile(Record *record){
    char * out = NULL;

    asprintf(&out,"insert %s;%s;%s;%d;%s;%d;%d;%d;%d\n",
             record->cdr_uniq_id, record->originator_number, record->destination_number,
             record->date, record->init_time, record->duration, record->type, record->tarrif,
             record->fault_condition);

    return out;
}

// delete record, free memory
void deleteRecord(Record *record) {
    free(record->cdr_uniq_id);
    free(record->originator_number);
    free(record->destination_number);
    free(record->init_time);

    free(record);
}
