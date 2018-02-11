#ifndef COORDINATOR_JOB_H
#define COORDINATOR_JOB_H

#include <signal.h>
#include <stdio.h>


typedef struct Job{

    pid_t pid;

    int id;
    int status;

    /*   STATUS
     *  Active      0
     *  Suspended   1
     *  Finished    2
     *  Terminated  3                     */

    int date;
    int submit_time;   ///  status-all since ...
    int last_start;   ///  active for ...

}Job;

// create new job
Job* newJob(pid_t pid, int id);
// print job
void printJob(Job *job);
// delete job
void deleteJob(Job *job);



#endif //COORDINATOR_JOB_H
