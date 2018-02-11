#ifndef JMS_POOL_JOB_H
#define JMS_POOL_JOB_H


#include <signal.h>

typedef struct Job{

    pid_t pid;
    int id;
    int status;

    /*   STATUS
     *  Active      0
     *  Suspended   1
     *  Finished    2
     *  Terminated  3                     */

}Job;


// create new job
Job* newJob(pid_t pid, int id);
// print job
void printJob(Job *job);
// delete job
void deleteJob(Job *job);

#endif //JMS_POOL_JOB_H
