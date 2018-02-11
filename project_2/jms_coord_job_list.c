#include <malloc.h>
#include "jms_coord_job_list.h"




// create list of jobs
JobList* newJobList(){
    JobList* list = malloc(sizeof(JobList));
    //printf("new list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;

    return list;
}

JobNode* newJobNode(JobList* list, Job* job){
    JobNode* node = malloc(sizeof(JobNode));
    //printf("new node @ %p\n",node);
    if(list!=NULL && job!=NULL){
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
      free(node);
      node=NULL;
    }

    return node;
}

// print list of jobs
void printJobList(JobList* list){
  printf("COORD: List of Jobs\n");
    if(list!=NULL){
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
  }
}
