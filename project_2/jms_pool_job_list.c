//
// Created by katerina on 28/4/2017.
//

#include "jms_pool_job_list.h"
#include "jms_pool_utilities.h"


// create list of jobs
JobList* newJobList(){
    JobList* list = malloc(sizeof(JobList));
    //printf("new list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;
    list->max_jobs=5;
    list->jobs_served=0;
    list->jobs_active=0;

    list->jms_in = -1;
    list->jms_out = -1;
    list->folder_path = NULL;

    return list;
}
JobNode* newJobNode(JobList* list, Job* job){
    JobNode* node = malloc(sizeof(JobNode));
    //printf("new node @ %p\n",node);
    if(list!=NULL){
      if(list->tail==NULL){   // if list is empty -> new root
          list->head = node;
          list->tail = node;
          node->next = NULL;
          node->previous = NULL;
          node->job = job;
      }else{
          list->tail->next = node;
          node->previous = list->tail;
          list->tail = node;
          node->next = NULL;
          node->job = job;
      }
    }else{
      printf("error: null list\n");
      free(node);
      node=NULL;
    }

    return node;
}

// print list of jobs
void printJobList(JobList* list){
  printf("\nPool: List of Jobs\n");
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        JobNode* node;
        node = list->head;
        while(node!=NULL){
            printJobNode(node);
            node = node->next;
        }
    }
}
void printJobNode(JobNode* node){
    printJob(node->job);
}

// delete list of jobs
void deleteJobList(JobList* list){
  if(list!=NULL){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        JobNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deleteJobNode(list,current);
            current=next;
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
    }
  }
    //printf("List deleted\n");
}

void deleteJobNode(JobList* list,JobNode* node){

  if(list!=NULL && node!=NULL){

    if(node->next==NULL){  // delete tail
        list->tail = node->previous;
    }

    if(node->previous==NULL){ // delete head
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{
        JobNode* previous = node->previous;
        JobNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }

    deleteJob(node->job);
    free(node);

  }else{
    //printf("error: null list or null node\n");
    ;
  }

}



Job* findJobWithID(int id){

    if(job_list->tail==NULL){
        return NULL;
    }else{
        JobNode* node;
        node = job_list->head;
        while(node!=NULL){
            if(node->job->id==id){
                return node->job;
            }
            node = node->next;
        }
    }
    return NULL;
}




Job* findJobWithPID(pid_t pid){
    if(job_list->tail==NULL){
        return NULL;
    }else{
        JobNode* node;
        node = job_list->head;
        while(node!=NULL){
            if(node->job->pid==pid){
                return node->job;
            }
            node = node->next;
        }
    }
    return NULL;
}


// kill all jobs in list
void killAllJobs(){

    if(job_list!=NULL){
        if(job_list->tail==NULL){
            printf("List empty\n");
        }else{
            JobNode* node;
            node = job_list->head;
            while(node!=NULL){
                if(node->job->status==0 || node->job->status==1){
                    kill(node->job->pid,SIGTERM);
              //      node->job->status=3;
                    // char to_coord[MAXBUFF]="";
                    // sprintf(to_coord,"Job %d %d %i",node->job->id,node->job->status,node->job->pid);
                    // WriteToPipe(job_list->jms_out,to_coord);
                    // kill(coord_pid,SIGUSR1);
                }
                node = node->next;
            }
        }
    }
    printJobList(job_list);

}
