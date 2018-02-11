#ifndef RECORD
#define RECORD

typedef struct Record{

    char* cdr_uniq_id;
    char* originator_number;
    char* destination_number;
    int   date;
    char* init_time;
    int   duration;
    int   type;
    int   tarrif;
    int   fault_condition;

}Record;

// create new empty record
Record* newEmpty();
// print record to stdout
void printRecord(Record *record);
// print record to string in file format -> for dump operation
char* printRecordtoFile(Record *record);
// delete record, free memory
void deleteRecord(Record *record);

#endif //RECORD
