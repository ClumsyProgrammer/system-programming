
#ifndef COORDINATOR_LISTOFJOBS_H
#define COORDINATOR_LISTOFJOBS_H


#include "jms_coord_job.h"

typedef struct JobNode{

    struct JobNode *next;
    struct JobNode *previous;
    Job* job;

}JobNode;

typedef struct JobList{

    JobNode* head;
    JobNode* tail;


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



#endif //COORDINATOR_LISTOFJOBS_H
