#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>

#include "UserInput.h"
#include "HashTable.h"
#include "Global.h"


void print_usage() {
    printf("./werhauz -o Operations -h1 Hashtable1NumOfEntries -h2 Hashtable2NumOfEntries -s BucketSize -c config-file -p PriceList\n");
}

// global struct
Info* info;

int main(int argc, char *argv[]) {

    //////   general variables
    int num_buckets_1=0;
    int num_buckets_2=0;
    int bucket_size=0;
    double price_array[5];


    //////////////////    c-getopt_long_only    /////////////////

    // https://linuxprograms.wordpress.com/2012/06/22/c-getopt_long_only-example-accessing-command-line-arguments/

    int opt= 0;

    int o_flag = 0;
    int h1_flag = 0;
    int h2_flag = 0;
    int c_flag = 0;
    int s_flag = 0;
    int p_flag = 0;

    char* o_value=malloc(sizeof(char)*LINE_MAX);
    char* h1_value=malloc(sizeof(char)*LINE_MAX);
    char* h2_value=malloc(sizeof(char)*LINE_MAX);
    char* c_value=malloc(sizeof(char)*LINE_MAX);
    char* s_value=malloc(sizeof(char)*LINE_MAX);
    char* p_value=malloc(sizeof(char)*LINE_MAX);


    //Specifying the expected options
    static struct option long_options[] = {
        {"o",      required_argument, 0,  'o' },
        {"h1",     required_argument, 0,  '1' },
        {"h2",     required_argument, 0,  '2' },
        {"c",      required_argument, 0,  'c' },
        {"s",      required_argument, 0,  's' },
        {"p",      required_argument, 0,  'p' },
        {0,        0,                 0,   0  }
    };

    int long_index =0;
    while ((opt = getopt_long_only(argc, argv,"",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 'o' :
                 o_flag=1;
                 strcpy(o_value,optarg);
                 break;
             case '1' :
                 h1_flag=1;
                 strcpy(h1_value,optarg);
                 break;
             case '2' :
                 h2_flag=1;
                 strcpy(h2_value,optarg);
                 break;
             case 'c' :
                 c_flag=1;
                 strcpy(c_value,optarg);
                 break;
             case 's' :
                 s_flag=1;
                 strcpy(s_value,optarg);
                 break;
             case 'p' :
                 p_flag=1;
                 strcpy(p_value,optarg);
                 break;
             default: print_usage();
                 exit(EXIT_FAILURE);
        }
    }

    printf("-----------------------------------------------------\n");


    printf("o_flag  = %d,  o_value   =  %s\n", o_flag,o_value);
    printf("h1_flag = %d,  h1_value  =  %s\n", h1_flag,h1_value);
    printf("h2_flag = %d,  h2_value  =  %s\n", h2_flag,h2_value);
    printf("c_flag  = %d,  c_value   =  %s\n", c_flag,c_value);
    printf("s_flag  = %d,  s_value   =  %s\n", s_flag,s_value);
    printf("p_flag  = %d,  p_value   =  %s\n", p_flag,p_value);

    printf("-----------------------------------------------------\n");



/////////     check arguments of main, ask for additional input         ////////////


    // number of buckets for hash table 1
    if(h1_flag==1){

      int i = atoi(h1_value);
      while(i<1){
        printf("Please insert valid number of buckets for hashtable 1:   " );
        scanf("%d",&i);
      }
      num_buckets_1=i;

    }else{
        int i=0;
        while(i<1){
          printf("Please insert valid number of buckets for hashtable 1:   " );
          scanf("%d",&i);
        }
        num_buckets_1=i;
    }


    // number of buckets for hash table 2
    if(h2_flag==1){

            int i = atoi(h2_value);
            while(i<1){
              printf("Please insert valid number of buckets for hashtable 2:   " );
              scanf("%d",&i);
            }
            num_buckets_2=i;
    }else{

      int i=0;
      while(i<1){
        printf("Please insert valid number of buckets for hashtable 2:   " );
        scanf("%d",&i);
      }
      num_buckets_2=i;
    }


    // configuration file -> not implemented
    if(c_flag==1){
        printf("Configuration file: %s\n",c_value);
    }else{
        printf("No configuration file given\n");
    }


    // size of bucket in bytes
    if(s_flag==1){

            int i = atoi(s_value);
            while(i<32){
              printf("Please insert valid size of bucket in bytes:   " );
              scanf("%d",&i);
            }
            bucket_size=i;
    }else{

      int i=0;
      while(i<32){
        printf("Please insert valid size of bucket in bytes:   " );
        scanf("%d",&i);
      }
      bucket_size=i;
    }


    // file for price list
    if(p_flag==1){

      printf("opening file: %s\n",p_value);
      if(ReadPriceFile(p_value,price_array)==-1){
          free(o_value);
          free(h1_value);
          free(h2_value);
          free(c_value);
          free(s_value);
          free(p_value);

          return 0;
      }

    }else{

      printf("Please insert file name for price list:   " );
      scanf("%s",p_value);
    }

    printf("Number of buckets for hash table 1: %d\n",num_buckets_1);
    printf("Number of buckets for hash table 2: %d\n",num_buckets_2);
    printf("Size of bucket for hash tables:     %d\n",bucket_size);
    printf("Price Array: %.2f, %.2f, %.2f, %.2f, %.2f\n",price_array[0],price_array[1],price_array[2],price_array[3],price_array[4]);

    printf("-----------------------------------------------------\n");

/////////////////////////////////////////////////////////////////////////////////////////////////
//////     Create Structures


    printf("-----------------------------------------------------\n");

    HashTable* caller_hash = newHashTable(num_buckets_1,bucket_size,0);
    printf("-----------------------------------------------------\n");

    HashTable* callee_hash = newHashTable(num_buckets_2,bucket_size,1);
    printf("-----------------------------------------------------\n");

    MaxTree* tree = newMaxTree();
    printf("-----------------------------------------------------\n");

    info=newInfo(price_array,caller_hash,callee_hash,tree);

/////////////////////////////////////////////////////////////////////////////////////////////////
//////    Process Input

    // load operations file
    if(o_flag!=1){
      printf("Awaiting operarions from standard input\n");
    }else{
      printf("Opening file: %s\n",o_value);
      int to_exit=0;
      to_exit=ReadFromFile(o_value);

    if(to_exit==1){

        /////////////////          wipe out        ////////////////
        //printf("-----------------------------------------------------\n");

        //printHashTable(caller_hash);
        //printHashTable(callee_hash);

        printf("WIPE OUT\n");


        printf("-----------------------------------------------------\n");
        deleteHashOnly(info->caller);
        printf("-----------------------------------------------------\n");
        freeHashTable(info->callee);
        printf("-----------------------------------------------------\n");
        deleteMaxTree(info->tree);
        printf("-----------------------------------------------------\n");
        deleteInfo(info);
        printf("-----------------------------------------------------\n");


        printf("RESTART\n");
        printf("-----------------------------------------------------\n");

        caller_hash = newHashTable(num_buckets_1,bucket_size,0);
        printf("-----------------------------------------------------\n");

        callee_hash = newHashTable(num_buckets_2,bucket_size,1);
        printf("-----------------------------------------------------\n");

        tree = newMaxTree();
        printf("-----------------------------------------------------\n");

        info=newInfo(price_array,caller_hash,callee_hash,tree);


    }else if(to_exit==2){

        /////////////////          terminate        ////////////////

        printf("EXIT\n");

        printf("-----------------------------------------------------\n");
        deleteHashOnly(info->caller);
        printf("-----------------------------------------------------\n");
        freeHashTable(info->callee);
        printf("-----------------------------------------------------\n");
        deleteMaxTree(info->tree);
        printf("-----------------------------------------------------\n");
        deleteInfo(info);
        printf("-----------------------------------------------------\n");

        free(o_value);
        free(h1_value);
        free(h2_value);
        free(c_value);
        free(s_value);
        free(p_value);

        return 0;


        }


    }


    ////////     wait for input from stdin       /////////////

    printf("\n\nProvide input:\n");


    int exit=0;

    while(exit!=2){
        char* str = malloc(LINE_MAX);
        fgets (str, LINE_MAX, stdin);
        if(str==NULL){
            printf("Please provide valid input:\n");
        }
	      char* op;
        op = strtok(str,"\n");

        if(op!=NULL)
            exit=ReadFromStandard(op);

        free(str);
        if(exit==1){

            /////////////////          wipe out        ////////////////

            printf("WIPE OUT\n");

            printf("-----------------------------------------------------\n");
            deleteInfo(info);
            printf("-----------------------------------------------------\n");
            deleteHashOnly(caller_hash);
            printf("-----------------------------------------------------\n");
            freeHashTable(callee_hash);
            printf("-----------------------------------------------------\n");
            deleteMaxTree(tree);
            printf("-----------------------------------------------------\n");


            printf("RESTART\n");
            printf("-----------------------------------------------------\n");

            caller_hash = newHashTable(num_buckets_1,bucket_size,0);
            printf("-----------------------------------------------------\n");

            callee_hash = newHashTable(num_buckets_2,bucket_size,1);
            printf("-----------------------------------------------------\n");

            tree = newMaxTree();
            printf("-----------------------------------------------------\n");

            info=newInfo(price_array,caller_hash,callee_hash,tree);


        }
    }



    /////////////////          exit        ////////////////


    printf("EXIT\n");
    printf("-----------------------------------------------------\n");
    deleteInfo(info);
    printf("-----------------------------------------------------\n");
    deleteHashOnly(caller_hash);
    printf("-----------------------------------------------------\n");
    freeHashTable(callee_hash);
    printf("-----------------------------------------------------\n");


    free(o_value);
    free(h1_value);
    free(h2_value);
    free(c_value);
    free(s_value);
    free(p_value);

    ////////////////////////////////////////////////////////////






    return 0;
}
