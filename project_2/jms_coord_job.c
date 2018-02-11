
#include <time.h>
#include <malloc.h>
#include "jms_coord_job.h"



// create new job
Job* newJob(pid_t pid, int id){

    Job* job = (Job *) malloc(sizeof (Job));

    job->id = id;
    job->pid = pid;

    job->status =0;

    ////    http://stackoverflow.com/questions/1442116/how-to-get-date-and-time-value-in-c-program
    time_t t = time(NULL);
    struct tm buf = *localtime(&t);

    job->date = (buf.tm_year + 1900)*10000 + (buf.tm_mon + 1)*100 + buf.tm_mday;
    job->submit_time =  buf.tm_hour*10000+buf.tm_min*100+buf.tm_sec;
    job->last_start = job->submit_time;

    return job;

}

// print job
void printJob(Job *job){
    printf("-->JOB: id: %d, pid: %i, status: %d, date: %8d, submit_time: %5d, last_start: %5d\n",
           job->id,job->pid,job->status,job->date,job->submit_time,job->last_start );
}

// delete job
void deleteJob(Job *job){
    free(job);
}
