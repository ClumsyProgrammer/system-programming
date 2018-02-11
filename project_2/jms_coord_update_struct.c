#include <time.h>
#include "jms_coord_update_struct.h"
#include "jms_coord_info.h"
#include "jms_coord_console_channel.h"


void CreateJob(char* input){

    info->job_sum++;

    Pool* pool = FindPoolToInsertJob();

    pool->jobs_served++;


    Job* new_job = newJob(-1,info->job_sum);
    newJobNode(pool->jobs,new_job);

    char to_pool[MAXBUFF]="";
    sprintf(to_pool,"%d ",info->job_sum);
    strcat(to_pool,input);
    //printf("COORD: message to pool: %s\n",to_pool);
    writeToPool(pool,to_pool);

}




Pool* FindPoolToInsertJob(){

  if(info!=NULL && info->list!=NULL){
    if(info->list->tail==NULL){
        return CreatePool();
    }else{
        if(info->list->tail->pool->jobs_served==info->list->tail->pool->size){
            return CreatePool();
        }else{
            return info->list->tail->pool;
        }
    }
  }

  return NULL;

}


Pool* CreatePool(){

    info->pool_sum++;
    Pool* new_pool = newPool(info->pool_sum);
    new_pool->pid = fork();


    if (new_pool->pid == -1){
        perror("Failed to fork");
        ShutDown();

    }else if (new_pool->pid == 0) {
        //printf("I am the pool: me: %lu --  parent: %lu\n", (long) getpid(), (long) getppid());

        ////////////////////////////////////////

        char *new_argv[10];
        //  "../jms_pool/cmake-build-debug/jms_pool"
        new_argv[0] = "./jms_pool";
        new_argv[1] = "-n";
        char str[15];
        sprintf(str, "%d", new_pool->size);
        new_argv[2] = str;
        new_argv[3] = "-p";
        new_argv[4] = info->path;
        new_argv[5] = "-r";
        new_argv[6] = new_pool->pipe_out;
        new_argv[7] = "-w";
        new_argv[8] = new_pool->pipe_in;
        new_argv[9] = NULL;

        execv(new_argv[0], new_argv);
        perror("execvp-> new pool ");
        exit(-1);
    }

    //////////   add pool to list and set channels
    //printPool(new_pool);
    newPoolNode(info->list,new_pool);
    linkToPool(new_pool);

    return new_pool;
}

Pool* FindPoolOfJob(int id){

    int pool_id = id%info->pool_size;

    if(info!=NULL && info->list!=NULL){
      if(info->list->tail==NULL){
          //printf("List empty\n");
          ;
      }else{
          PoolNode* node;
          node = info->list->head;
          while(node!=NULL){
              if(node->pool->id==pool_id){
                  return node->pool;
              }else{
                  node = node->next;
              }
          }
      }
    }

    return NULL;
}



Job* FindJob(int id){
    Pool* pool = FindPoolOfJob(id);
    if(pool!=NULL){
      JobList* list = pool->jobs;
      if(list!=NULL){
          if(list->tail==NULL){
              WriteToConsole("No jobs available");
          }else{
              JobNode* node;
              node = list->head;
              while(node!=NULL){
                  if(node->job->id==id){
                      return node->job;
                  }else{
                      node = node->next;
                  }
              }
          }
      }
    }

    return NULL;
}


void GetJobStatus(int id, int lapsed, int status){


  time_t t = time(NULL);
  struct tm buf = *localtime(&t);
  int now_time =  buf.tm_hour*10000+buf.tm_min*100+buf.tm_sec;

    if(id!=0){
        // show only one
        Job* job = FindJob(id);
        char to_cons[MAXBUFF]="";
        if(job!=NULL){

            if(job->status==0){
                int secs = now_time - job->last_start;
                sprintf(to_cons,"JobID %d Status: Active (running for %d seconds)",job->id,secs);
                WriteToConsole(to_cons);
            }else if(job->status==1){
                sprintf(to_cons,"JobID %d Status: Suspended",job->id);
                WriteToConsole(to_cons);
            }else if(job->status==2){
                sprintf(to_cons,"JobID %d Status: Finished",job->id);
                WriteToConsole(to_cons);
            }

        }else{
            strcpy(to_cons,"No job found");
        }


    }else{
        if(lapsed!=0){
           // show all since...
           char to_cons[MAXBUFF]="";
           int after = now_time - lapsed;

            if(info!=NULL && info->list!=NULL){

              if(info->list->tail==NULL){
                  //printf("List of pools empty\n");
                  strcpy(to_cons,"No jobs found");
                  WriteToConsole(to_cons);
              }else{
                  PoolNode* node;
                  node = info->list->head;
                  while(node!=NULL){
                    if(node->pool!=NULL){

                      if(node->pool->jobs!=NULL){
                          if(node->pool->jobs->tail==NULL){
                              //printf("List empty\n");
                              ;
                          }else{
                              JobNode* node2;
                              node2 = node->pool->jobs->head;
                              while(node2!=NULL){
                                   if(node2->job->submit_time>after){
                                     if(node2->job->status==0){
                                         int secs = now_time - node2->job->last_start;
                                         sprintf(to_cons,"JobID %d Status: Active (running for %d seconds)",node2->job->id,secs);
                                         WriteToConsole(to_cons);
                                     }else if(node2->job->status==1){
                                         sprintf(to_cons,"JobID %d Status: Suspended",node2->job->id);
                                         WriteToConsole(to_cons);
                                     }else if(node2->job->status==2){
                                         sprintf(to_cons,"JobID %d Status: Finished",node2->job->id);
                                         WriteToConsole(to_cons);
                                     }

                                   }
                                   // next job
                                  node2 = node2->next;
                              }
                          }
                      }

                    }

                      // next pool
                      node = node->next;
                  }
              }

            }



        }else{
            if(status==0){
                // show all

                char to_cons[MAXBUFF]="";

                if(info!=NULL && info->list!=NULL){

                  if(info->list->tail==NULL){
                      printf("List of pools empty\n");
                      strcpy(to_cons,"No jobs found");
                      WriteToConsole(to_cons);
                  }else{
                      PoolNode* node;
                      node = info->list->head;
                      while(node!=NULL){
                        if(node->pool!=NULL){

                          if(node->pool->jobs!=NULL){
                              if(node->pool->jobs->tail==NULL){
                                  printf("Job List empty\n");
                              }else{
                                  JobNode* node2;
                                  node2 = node->pool->jobs->head;
                                  while(node2!=NULL){

                                         if(node2->job->status==0){
                                             int secs = now_time - node2->job->last_start;
                                             sprintf(to_cons,"JobID %d Status: Active (running for %d seconds)",node2->job->id,secs);
                                             WriteToConsole(to_cons);
                                         }else if(node2->job->status==1){
                                             sprintf(to_cons,"JobID %d Status: Suspended",node2->job->id);
                                             WriteToConsole(to_cons);
                                         }else if(node2->job->status==2){
                                             sprintf(to_cons,"JobID %d Status: Finished",node2->job->id);
                                             WriteToConsole(to_cons);
                                         }


                                       // next job
                                      node2 = node2->next;
                                  }
                              }
                          }

                        }

                          // next pool
                          node = node->next;
                      }
                  }

                }






            }else if(status==1){
                // show active



                char to_cons[MAXBUFF]="";

                if(info!=NULL && info->list!=NULL){

                  if(info->list->tail==NULL){
                      printf("List of pools empty\n");
                      strcpy(to_cons,"No jobs found");
                      WriteToConsole(to_cons);
                  }else{
                      PoolNode* node;
                      node = info->list->head;
                      while(node!=NULL){
                        if(node->pool!=NULL){

                          if(node->pool->jobs!=NULL){
                              if(node->pool->jobs->tail==NULL){
                                  printf("Job List empty\n");
                              }else{
                                  JobNode* node2;
                                  node2 = node->pool->jobs->head;
                                  while(node2!=NULL){

                                         if(node2->job->status==0){
                                             int secs = now_time - node2->job->last_start;
                                             sprintf(to_cons,"JobID %d Status: Active (running for %d seconds)",node2->job->id,secs);
                                             WriteToConsole(to_cons);
                                         }


                                       // next job
                                      node2 = node2->next;
                                  }
                              }
                          }

                        }

                          // next pool
                          node = node->next;
                      }
                  }

                }



            }else if(status==2){
                // show finished


                char to_cons[MAXBUFF]="";

                if(info!=NULL && info->list!=NULL){

                  if(info->list->tail==NULL){
                      printf("List of pools empty\n");
                      strcpy(to_cons,"No jobs found");
                      WriteToConsole(to_cons);
                  }else{
                      PoolNode* node;
                      node = info->list->head;
                      while(node!=NULL){
                        if(node->pool!=NULL){

                          if(node->pool->jobs!=NULL){
                              if(node->pool->jobs->tail==NULL){
                                  printf("Job List empty\n");
                              }else{
                                  JobNode* node2;
                                  node2 = node->pool->jobs->head;
                                  while(node2!=NULL){

                                         if(node2->job->status==2){
                                             sprintf(to_cons,"JobID %d Status: Finished",node2->job->id);
                                             WriteToConsole(to_cons);

                                         }


                                       // next job
                                      node2 = node2->next;
                                  }
                              }
                          }

                        }

                          // next pool
                          node = node->next;
                      }
                  }

                }





            }
        }
    }



}

void GetPoolStatus(){

  if(info!=NULL && info->list!=NULL){

    if(info->list->tail==NULL){
        WriteToConsole("No pools available");
    }else{
        WriteToConsole("Pools & NumOfJobs:");
        PoolNode* node;
        node = info->list->head;
        while(node!=NULL){
            if(node->pool->status==0 && node->pool->active_jobs>0){
                char to_cons[MAXBUFF]="";
                sprintf(to_cons,"%i %d",node->pool->pid,node->pool->active_jobs);
                WriteToConsole(to_cons);

            }
                node = node->next;

        }
    }

  }


return;

}



void UpdatePool(Pool* pool,int status){
  pool->pid = -1;
  pool->status = status;
  disconnectPool(pool);
}



void UpdateJob(char* message){

  printf("UPDATE JOB:  __%s__\n",message);
  time_t t = time(NULL);
  struct tm buf = *localtime(&t);
  int now_time =  buf.tm_hour*10000+buf.tm_min*100+buf.tm_sec;

  char operation[MAXBUFF]="";
  strcpy(operation,message);
  if(operation!=NULL){

    char *pch;
    pch = strtok(operation, " ");

    if(pch!=NULL){

      pch = strtok(NULL, " ");

      if(pch!=NULL){

        int job_id = atoi(pch);
        pch = strtok(NULL, " ");

        if(pch!=NULL){

          int job_status = atoi(pch);
          Job* job = FindJob(job_id);
          char to_cons[MAXBUFF]="";

          if(job!=NULL){

            job->status = job_status;
            if(job_status==0){
              job->last_start = now_time;
              pch = strtok(NULL, " ");
              if(pch!=NULL){
                pid_t job_pid = atol(pch);
                job->pid = job_pid;
              }
            }

            // inform console


            Pool* pool = FindPoolOfJob(job->id);
            if(pool!=NULL){
              if(job->status==0){
                  int secs = now_time - job->last_start;
                  pool->active_jobs++;
                  sprintf(to_cons,"JobID %d Status: Active (running for %d seconds)",job->id,secs);
                  WriteToConsole(to_cons);
              }else if(job->status==1){
                pool->active_jobs--;
                  sprintf(to_cons,"JobID %d Status: Suspended",job->id);
                  WriteToConsole(to_cons);
              }else if(job->status==2){
                pool->active_jobs--;
                  sprintf(to_cons,"JobID %d Status: Finished",job->id);
                  WriteToConsole(to_cons);
              }
            }
          }else{
            strcpy(to_cons,"No job found\n");
          }


        }
      }
    }
  }
}




void ShutDown(){
  if(info!=NULL && info->list!=NULL){

    if(info->list->tail==NULL){
        printf("List empty\n");
    }else{
        PoolNode* node;
        node = info->list->head;
        while(node!=NULL){

            if(node->pool->status==0){
              node->pool->status=2;
              kill(node->pool->pid,SIGTERM);
              int st;
              waitpid(node->pool->pid, &st, 0);
              disconnectPool(node->pool);
            }

            node = node->next;
        }
    }

    info->mode=0;

  }



}
