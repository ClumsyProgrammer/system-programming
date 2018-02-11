#include "UserInput.h"
#include "CallRecord.h"
#include "HashFunction.h"
#include "Global.h"
#include "RecordManagement.h"
#include "Statistics.h"
#include <string.h>
#include <limits.h>


//////     http://stackoverflow.com/questions/2396623/remove-new-line-character-in-c


//To remove first occurrence of new line char in string, use:
//
//char *pch = strstr(myStr, "\n");
//if(pch != NULL)
//strncpy(pch, "\0", 1);


////////     line[read - 2] = 0;



//////////   Process Operations File    ///////////////

int ReadFromFile(char* FileName){

    int exit=0;
    // Υλοποίηση Συστημάτων Βάσεων Δεδομένων
    FILE *stream = NULL;

    char *line;
    size_t len = 0;
    ssize_t read;

    stream = fopen(FileName,"r");
    //printf("csv open: %p\n",stream);

    if(stream!=NULL){
        // read file line by line
        read = getline(&line, &len, stream);
        while (read != -1){
            line[read - 1] = 0;  // remove \n character
            exit=ReadFromStandard(line);  // process every line
            read = getline(&line, &len, stream); // read next line

        }


        fclose(stream);


    }else{
        printf("Error opening file: %s\n",FileName);
        exit = 2;
    }
    free(line);
    return exit;

}




/////////////  Process a string  ///////////////////////

int ReadFromStandard(char *line){
    // function variables
    char* operation=malloc(sizeof(char*)*(strlen(line)+1));
    int exit=0;
    char *pch;
    // parse line
    pch = strtok(line, " ");
    if(pch==NULL){
        printf("Please provide valid input:\n");
        return exit;
    }
    strcpy(operation,pch);
    printf("\n-> %s \n",operation);

    if(strcmp(operation,"insert")==0){
        Record* record=newEmpty();
        // parse and copy fields to record
        pch = strtok(NULL, ";");
        record->cdr_uniq_id=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(record->cdr_uniq_id, pch);

        pch = strtok(NULL, ";");
        record->originator_number=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(record->originator_number, pch);

        pch = strtok(NULL, ";");
        record->destination_number=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(record->destination_number, pch);

        pch = strtok(NULL, ";");
        record->date=atoi(pch);

        pch = strtok(NULL, ";");
        record->init_time=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(record->init_time, pch);

        pch = strtok(NULL, ";");
        record->duration=atoi(pch);

        pch = strtok(NULL, ";");
        record->type=atoi(pch);

        pch = strtok(NULL, ";");
        record->tarrif=atoi(pch);

        pch = strtok(NULL, ";");
        record->fault_condition=atoi(pch);

        // insert record to structures

        int h1=hashFunction(info->caller->numberOfBuckets,record->originator_number);
        int h2=hashFunction(info->callee->numberOfBuckets,record->destination_number);
        insertRecordtoHash(info->caller,record,h1);
        insertRecordtoHash(info->callee,record,h2);
        insertRecordtoTree(record);
        //printRecord(record);

    }else if(strcmp(operation,"delete")==0){
        // parse arguments
        pch = strtok(NULL, " ");
        char* id=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(id, pch);
        printf("id = %s\n", id);
        pch = strtok(NULL, " ");
        char* caller=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(caller, pch);
        printf("caller = %s\n", caller );
        // remove record
        int del = removeRecordfromHash(info->caller,caller,id);
        if(del==0){
            printf("Record removed successfully\n");
        }else if(del==1){
            printf("No record found\n");
        }else{
            printf("Error while removing record\n");
        }
        // free strings
        free(id);
        free(caller);

    }else if(strcmp(operation,"find")==0){
        // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

        ListOfRecords* temp;
        char *token[7];
        int i=0;
        /* get the first token */
        token[i] = strtok(NULL, " ");
        /* walk through other tokens */
        while( token[i] != NULL ) {
            i++;
            token[i] = strtok(NULL, " ");
        }

        //printf("type of find\n");

        if(i==1){

            //printf("caller only\n");
            char* caller_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(caller_num,token[0]);
            printf("caller_num: %s\n",caller_num);
            // create list with results and print it
            temp=findRecordsinHash(info->caller,caller_num,0,0,NULL,NULL);
            printf("Records found:\n");
            printList(temp);

            free(caller_num);

        }else if(i==3){

            //printf("caller and time or date\n");
            char* caller_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(caller_num,token[0]);
            printf("caller_num: %s\n",caller_num);
            if(strchr(token[1],':')==NULL){
                int date_from=atoi(token[1]);
                printf("date_from: %d\t",date_from);
                int date_to=atoi(token[2]);
                printf("date_to: %d\n",date_to);
                // create list with results
                temp=findRecordsinHash(info->caller,caller_num,date_from,date_to,NULL,NULL);

            }else{
                char* time_from=malloc(sizeof(char*)*(strlen(token[1])+1));
                strcpy(time_from,token[1]);
                printf("time_from: %s\t",time_from);
                char* time_to=malloc(sizeof(char*)*(strlen(token[2])+1));
                strcpy(time_to,token[2]);
                printf("time_to: %s\n",time_to);
                // create list with results
                temp=findRecordsinHash(info->caller,caller_num,0,0,time_from,time_to);
                free(time_from);
                free(time_to);

            }

            printf("Records found:\n");
            printList(temp);
            freeList(temp);

            free(caller_num);

        }else if(i==5){

            //printf("caller and time and date\n");
            char* caller_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(caller_num,token[0]);
            printf("caller_num: %s\n",caller_num);

            char* time_from=malloc(sizeof(char)*(strlen(token[1])+1));
            strcpy(time_from,token[1]);
            printf("time_from: %s\t",time_from);
            int date_from = atoi(token[2]);
            printf("date_from: %d\t",date_from);

            char* time_to=malloc(sizeof(char)*(strlen(token[3])+1));
            strcpy(time_to,token[3]);
            printf("time_to: %s\t",time_to);
            int date_to = atoi(token[4]);
            printf("date_to: %d\n",date_to);
            // create list with results
            temp = findRecordsinHash(info->caller,caller_num,date_from,date_to,time_from,time_to);
            printf("Records found:\n");
            printList(temp);
            freeList(temp);

            free(caller_num);
            free(time_from);
            free(time_to);

        }

    }else if(strcmp(operation,"lookup")==0){
        // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

        ListOfRecords* temp;
        char *token[7];
        int i=0;
        /* get the first token */
        token[i] = strtok(NULL, " ");
        /* walk through other tokens */
        while( token[i] != NULL ) {
            i++;
            token[i] = strtok(NULL, " ");
        }

        //printf("type of lookup\n");

        if(i==1){
            //printf("callee only\n");
            char* callee_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(callee_num,token[0]);
            printf("callee_num: %s\n",callee_num);
            // create list with results
            temp=findRecordsinHash(info->callee,callee_num,0,0,NULL,NULL);
            printf("Records found:\n");
            printList(temp);

            free(callee_num);

        }else if(i==3){

            //printf("callee and time or date\n");
            char* callee_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(callee_num,token[0]);
            printf("callee_num: %s\n",callee_num);

            if(strchr(token[1],':')==NULL){
                int date_from=atoi(token[1]);
                printf("date_from: %d\t",date_from);
                int date_to=atoi(token[2]);
                printf("date_to: %d\n",date_to);
                // create list with results
                temp=findRecordsinHash(info->callee,callee_num,date_from,date_to,NULL,NULL);

            }else{
                char* time_from=malloc(sizeof(char)*(strlen(token[1])+1));
                strcpy(time_from,token[1]);
                printf("time_from: %s\t",time_from);
                char* time_to=malloc(sizeof(char)*(strlen(token[2])+1));
                strcpy(time_to,token[2]);
                printf("time_to: %s\n",time_to);
                // create list with results
                temp=findRecordsinHash(info->callee,callee_num,0,0,time_from,time_to);
                free(time_from);
                free(time_to);

            }

            printf("Records found:\n");
            printList(temp);
            freeList(temp);
            free(callee_num);


        }else if(i==5){
            //printf("callee and time and date\n");
            char* callee_num=malloc(sizeof(char)*(strlen(token[0])+1));
            strcpy(callee_num,token[0]);
            printf("callee_num: %s\n",callee_num);

            char* time_from=malloc(sizeof(char)*(strlen(token[1])+1));
            strcpy(time_from,token[1]);
            printf("time_from: %s\t",time_from);
            int date_from=atoi(token[2]);
            printf("date_from: %d\t",date_from);

            char* time_to=malloc(sizeof(char)*(strlen(token[3])+1));
            strcpy(time_to,token[3]);
            printf("time_to: %s\t",time_to);
            int date_to=atoi(token[4]);
            printf("date_to: %d\n",date_to);
            // create list with results
            temp=findRecordsinHash(info->callee,callee_num,date_from,date_to,time_from,time_to);
            printf("Records found:\n");
            printList(temp);
            freeList(temp);

            free(callee_num);
            free(time_from);
            free(time_to);

       }

    }else if(strcmp(operation,"indist")==0){

        pch = strtok(NULL, " ");
        char* caller1=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(caller1, pch);
        printf("caller1 = %s\n", caller1);
        pch = strtok(NULL, " ");
        char* caller2=malloc(sizeof(char)*(strlen(pch)+1));
        strcpy(caller2, pch);
        printf("caller2 = %s\n", caller2 );

        // control given to other function
        findSetOfEntries(caller1,caller2);

        free(caller1);
        free(caller2);

    }else if(strcmp(operation,"topdest")==0){

        pch = strtok(NULL, " ");
        char* caller=malloc(sizeof(char)*(strlen(pch)+1));

        strcpy(caller, pch);
        printf("caller = %s\n", caller );

        // control given to other function
        findTopDest(caller);

        free(caller);

    }else if(strcmp(operation,"top")==0){

        pch = strtok(NULL, " ");
        int i = atoi(pch);
        printf("top k = %d\n", i );
        // control given to other function
        findTopCaller(i);

    }else if(strcmp(operation,"bye")==0){

        // refresh all structures
        exit=1;
        printf("Wipe out\n");

    }else if(strcmp(operation,"exit")==0){

        // release all structures
        exit=2;
        printf("Begin preparations to exit\n");

    }else if(strcmp(operation,"print")==0){

        pch = strtok(NULL, " ");
        char to_print[25]="";
        strcpy(to_print,pch);
        printf("to print = %s\n", to_print );
        if(strstr(to_print, "hash") != NULL) {
            if(strstr(to_print, "1") != NULL) {
                printHashTable(info->caller);
            }else if(strstr(to_print, "2") != NULL) {
                printHashTable(info->callee);
            }else{
                printf("Unknown argument\n");
            }
        }else if(strstr(to_print, "tree") != NULL){
            // vertical print of tree
            printMaxTreeNode(info->tree->root,0);
        }else{
            printf("Unknown argument\n");
        }

    }else if(strcmp(operation,"dump")==0){

        // print stuctures to file
        // output can be given as input
        pch = strtok(NULL, " ");
        char to_print[25]="";
        strcpy(to_print,pch);
        printf("to save = %s\n", to_print );
        if(strstr(to_print, "hash") != NULL) {
            if(strstr(to_print, "1") != NULL) {
                pch = strtok(NULL, " ");
                if(pch!=NULL){
                    strcpy(to_print,pch);
                    printf("to location = %s\n", to_print );
                    printHashtoFile(info->caller,to_print);
                }else{
                    printf("Please retry with valid destination filename\n");
                }
            }else if(strstr(to_print, "2") != NULL) {
                pch = strtok(NULL, " ");

                if(pch!=NULL){
                    strcpy(to_print,pch);
                    printf("to location = %s\n", to_print );
                    printHashtoFile(info->callee,to_print);
                }else{
                    printf("Please retry with valid destination filename\n");
                }
            }else{
                printf("Unknown argument\n");
            }
        }else{
            printf("Unknown argument\n");
        }


    }else{

        printf("\nUnknown command, will be discarded\n");

    }

    free(operation);

    return exit;
}






int ReadPriceFile(char* filename, double price[5]){

  // read templated file for price list

    FILE *stream = NULL;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    stream = fopen(filename,"r");
    //printf("file open: %p\n",stream);

    if(stream!=NULL){

        int k=0;
        read = getline(&line, &len, stream);
        while (read != -1) {
            k++;
            char *token[4];
            int i=0;
            /* get the first token */
            token[i] = strtok(line, ";");
            /* walk through other tokens */
            while( token[i] != NULL ) {
                i++;
                token[i] = strtok(NULL, ";");
            }

            if(k>1){
                price[k-2]=atof(token[2]);
            }
            //free(line);
            read = getline(&line, &len, stream);
        }

        fclose(stream);
        free(line);
        return 0;;


    }else{
        printf("Error opening file: %s\n",filename);
        return -1;
    }



}
