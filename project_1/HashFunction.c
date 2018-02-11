#include <stdio.h>
#include <string.h>
#include "HashFunction.h"

int hashFunction(int buckets, char* string){
    // copy string to process
    char* temp=malloc(sizeof(char)*(strlen(string)+1));
    strcpy(temp,string);
    char *pch;//=malloc(sizeof(char)*(sizeof(string)+1));
    // get first 3 digits
    pch = strtok(temp, "-");
    long long int i;
    i = atoll(pch)*10000000000;
    // get the rest of digits
    pch = strtok(NULL, "-");
    i= i + atoll(pch);
    // find target bucket with %
    i=i%buckets;
    // release temp
    free(temp); 

    //printf("Going to bucket: %d\n",(int)i);
    return (int)i;
}
