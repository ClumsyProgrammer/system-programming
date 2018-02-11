//
// Created by katerina on 17/4/2017.
//

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {


    time_t t1 = time(NULL);
    struct tm buf1 = *localtime(&t1);

    int stime =  buf1.tm_hour*10000+buf1.tm_min*100+buf1.tm_sec;

    fprintf(stderr, "hello.c -> id: %i  start: %d\n",getpid(), stime);
    printf( "hello.c -> id: %i parent: %i start: %d\n",getpid(), getppid(), stime);


    if(argc == 1){

        printf("Going to sleep for 5 secs... \n");
        int n;
        for(n=0;n<5;n++){
            sleep(1);
        }

    }else{

        int nap = atoi(argv[1]);
        printf("Going to sleep for %d secs... \n",nap);
        int n;
        for(n=0;n<nap;n++){
            sleep(1);
        }
    }



    time_t t2 = time(NULL);
    struct tm buf2 = *localtime(&t2);

    int ftime =  buf2.tm_hour*10000+buf2.tm_min*100+buf2.tm_sec;

    fprintf(stderr, "hello.c -> id: %i  finish: %d\n",getpid(), ftime);
    printf( "hello.c -> id: %i parent: %i finish: %d\n",getpid(), getppid(), ftime);


    return 0;



}