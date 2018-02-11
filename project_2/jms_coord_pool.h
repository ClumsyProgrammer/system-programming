#ifndef COORDINATOR_POOL_H
#define COORDINATOR_POOL_H

#include <signal.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "jms_coord_job_list.h"
#include "jms_coord_utilities.h"

#define MAXBUFF 1024
#define FIFO3   "/pool_fifo_1"
#define FIFO4   "/pool_fifo_2"

typedef struct Pool{

    pid_t pid;
    int id;

    char pipe_in[150];
    char pipe_out[150];
    int fd_in;
    int fd_out;

    int size;
    int status;

    /*   STATUS
     *
     *  Prepare channel  -1
     *  Active           0
     *  Finished         1
     *  Terminated       2    */

    int jobs_served;
    int active_jobs;

    JobList* jobs;

}Pool;

// create new pool
Pool* newPool(int id);

// print pool
void printPool(Pool *pool);

// create communication channel
void linkToPool(Pool *pool);

// open communication channel
void writeToPool(Pool *pool,char* message);

// disconnect pool
void disconnectPool(Pool *pool);

// delete pool
void deletePool(Pool *pool);


#endif //COORDINATOR_POOL_H
