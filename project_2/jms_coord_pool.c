#include "jms_coord_pool.h"
#include "jms_coord_info.h"

// create new pool
Pool* newPool(int id){
    Pool* pool = (Pool *) malloc(sizeof (Pool));

    pool->pid = -1;
    pool->id=id;
    pool->size = info->pool_size;

    pool->status = -1;
    pool->jobs_served = 0;
    pool->active_jobs = 0;

    pool->jobs = newJobList();

    pool->fd_in = -1;
    pool->fd_out = -1;

    char buff[20];
    sprintf(buff, "_%d", id); // puts string into buffer
    strcpy(pool->pipe_in, info->path);
    strcat(pool->pipe_in, FIFO3);
    strcat(pool->pipe_in, buff);
    strcpy(pool->pipe_out, info->path);
    strcat(pool->pipe_out, FIFO4);
    strcat(pool->pipe_out, buff);

    CreateCommunicationChannelToProcess(&(pool->fd_in), &(pool->fd_out), pool->pipe_in, pool->pipe_out);

    return pool;
}
// print pool
void printPool(Pool *pool){
    printf("-> POOL no %d\npid: %i, status: %d, size: %d, jobs_served: %d, active_jobs: %d\n",
           pool->id,pool->pid,pool->status,pool->size,pool->jobs_served,pool->active_jobs );
    printf("fd_in: %i, fd_out: %d, pipe_in: %s, pipe_out: %s\n",
           pool->fd_in,pool->fd_out,pool->pipe_in,pool->pipe_out );
    printJobList(pool->jobs);
}

// open communication channel
void linkToPool(Pool *pool){
    printf("jms_coord: wait for pool to open write pipe\n");
    int i=0;
    char message[MAXBUFF]="";
    read(pool->fd_in, message, MAXBUFF);
    while(strcmp(message,"ready")!=0&&i<5){   // wait up to 5secs for pool to respond
        sleep(1);
        i++;
        read(pool->fd_in, message, MAXBUFF);
    }
    if(i==6&&strcmp(message,"ready")!=0){
        printf("jms_coord: pool not responding\n"); // exit coord because of pool error
        info->mode=0;
    }else{
        OpenWritePipe(&(pool->fd_out), pool->pipe_out);
        pool->status=0;
        //printPool(pool);
    }
}



void writeToPool(Pool *pool,char* message){
    WriteToPipe(pool->fd_out,message);
    kill(pool->pid,SIGUSR1);
}

// disconnect pool
void disconnectPool(Pool *pool){
    DeleteCommunicationChannelToProcess(pool->fd_in,pool->fd_out,pool->pipe_in,pool->pipe_out);
}

// delete pool
void deletePool(Pool *pool){
    deleteJobList(pool->jobs);
    free(pool);
}
