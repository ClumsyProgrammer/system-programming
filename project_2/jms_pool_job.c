//
// Created by katerina on 27/4/2017.
//

#include "jms_pool_job.h"



#include <time.h>
#include <malloc.h>
#include "jms_pool_job.h"



// create new job
Job* newJob(pid_t pid, int id){

    Job* job = (Job *) malloc(sizeof (Job));

    job->id = id;
    job->pid = pid;
    job->status=0;

    /*   STATUS
     *  Active      0
     *  Suspended   1
     *  Finished    2
     *  Terminated  3                     */




    return job;

}

// print job
void printJob(Job *job){
    printf("->JOB: id: %d, pid: %i status: %d\n",
           job->id,job->pid,job->status);
}

// delete job
void deleteJob(Job *job){
    free(job);
}
