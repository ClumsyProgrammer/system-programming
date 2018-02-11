//
// Created by katerina on 28/4/2017.
//

#ifndef JMS_POOL_LISTOFJOBS_H
#define JMS_POOL_LISTOFJOBS_H


#include <malloc.h>
#include "jms_pool_job.h"

typedef struct JobNode{

    struct JobNode *next;
    struct JobNode *previous;
    Job* job;

}JobNode;

typedef struct JobList{

    JobNode* head;
    JobNode* tail;
    int max_jobs;
    int jobs_served;
    int jobs_active;

    int jms_in;
    int jms_out;
    char* folder_path;


}JobList;

// create list of jobs
JobList* newJobList();
JobNode* newJobNode(JobList* list, Job* job);

// print list of jobs
void printJobList(JobList* list);
void printJobNode(JobNode* node);

// delete list of jobs
void deleteJobList(JobList* list);
void deleteJobNode(JobList* list,JobNode* node);

//find job in list
Job* findJobWithID(int id);
Job* findJobWithPID(pid_t pid);

// kill all jobs in list
void killAllJobs();

#endif //JMS_POOL_LISTOFJOBS_H
