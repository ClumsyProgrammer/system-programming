//
// Created by katerina on 27/4/2017.
//

#include <time.h>
#include "jms_pool_utilities.h"



void OpenCommunicationChannel(int *read_pipe, int *write_pipe, char* read_fifo, char* write_fifo){


    // open pipes

    *read_pipe = open(read_fifo, O_RDONLY | O_NONBLOCK);     //Open for reading only.
    if(*read_pipe >= 0)						//Upon successful completion, open function return a non-negative integer
        //printf("pool: read pipe opened %s\n",read_fifo);
        ;
    else{
        //fprintf(stderr, "pool_utilities: read pipe open ERROR\n");
        perror("pool_utilities: read pipe open");
        terminate=1;
        //exit(1);
    }

    *write_pipe = open(write_fifo, O_WRONLY | O_NONBLOCK);     //Open for writing only.
    if(*write_pipe >= 0)						//Upon successful completion, open function return a non-negative integer
        //printf("pool: write pipe opened %s\n",write_fifo);
        ;
    else{
        //fprintf(stderr, "pool_utilities: write pipe open ERROR\n");
        perror("pool_utilities: write pipe open");
        terminate=1;
        //exit(1);
    }



}



void CloseCommunicationChannel(int readfd, int writefd){


    // close fifos

    if(close(readfd) == 0)
        //printf("pool_utilities: read pipe closed\n");
        ;
    else{
        //fprintf(stderr, "pool_utilities: read pipe close ERROR\n");
        perror("pool_utilities: read pipe close");
        terminate=1;
        //exit(1);
    }

    if(close(writefd) == 0)
        //printf("pool_utilities: write pipe closed\n");
        ;
    else{
        //fprintf(stderr, "pool_utilities: write pipe close ERROR\n");
        perror("pool_utilities: write pipe close");
        terminate=1;
        //exit(1);
    }



}




void ReadFromPipe(int readfd){

    // apo diafaneies
    char message[MAXBUFF]="";
    ssize_t n = read(readfd, message, MAXBUFF);

    if(n>0){

        message[n] = '\0';
        char operation[MAXBUFF];
        strcpy(operation,message);
        char *pch;
        pch = strtok(operation, " ");

        if(strcmp(pch,"shutdown")==0){
            killAllJobs();
            terminate=1;
        }else if(strcmp(pch,"suspend")==0){
            pch = strtok(operation, " ");
            int job_id = atoi(pch);
            Job* job = findJobWithID(job_id);
            kill(job->pid,SIGSTOP);
            job->status=1;
            char to_coord[MAXBUFF]="";
            sprintf(to_coord,"Job %d %d %i",job->id,job->status,job->pid);
            WriteToPipe(job_list->jms_out,to_coord);
            kill(coord_pid,SIGUSR1);
        }else if(strcmp(pch,"resume")==0){
            pch = strtok(operation, " ");
            int job_id = atoi(pch);
            Job* job = findJobWithID(job_id);
            kill(job->pid,SIGCONT);
            job->status=0;
            char to_coord[MAXBUFF]="";
            sprintf(to_coord,"Job %d %d %i",job->id,job->status,job->pid);
            WriteToPipe(job_list->jms_out,to_coord);
            kill(coord_pid,SIGUSR1);
        }else if(strcmp(pch,"exit")==0){
            terminate=1;
        }else {
            printf("%s\n",message);

            char *p = strtok(message, " ");
            int new_job_id = atoi(p);
            p = strtok(NULL, " "); ///   "submit" -> ignored

            printf("id=%d_______%s_______  same: %d\n",new_job_id,p,strcmp(p,"submit"));
            if(strcmp(p,"submit")==0){



              /////////   execute job

              job_list->jobs_served++;
              job_list->jobs_active++;
              pid_t  childpid1;
              childpid1 = fork();


              if (childpid1 == -1){
                  perror("Failed to fork");

              }else if (childpid1 == 0) {

                  printf("JOB  id: %d  pid: %i\n",new_job_id,getpid());

                  ///////  http://stackoverflow.com/questions/11198604/c-split-string-into-an-array-of-strings
                  char **res = NULL;
                  p = strtok(NULL, " ");
                  int n_spaces = 0;

                  /* split string and append tokens to 'res' */
                  while (p) {
                      res = realloc(res, sizeof(char *) * ++n_spaces);
                      if (res == NULL)
                          exit(-1); /* memory allocation failed */
                      res[n_spaces - 1] = p;
                      p = strtok(NULL, " ");
                  }

                  /* realloc one extra element for the last NULL */
                  res = realloc(res, sizeof(char *) * (n_spaces + 1));
                  res[n_spaces] = 0;



                  /* create directory and files for output */

                  struct stat st = {0};
                  time_t t = time(NULL);
                  struct tm buf = *localtime(&t);
                  int sub_date = (buf.tm_year + 1900)*10000 + (buf.tm_mon + 1)*100 + buf.tm_mday;
                  int sub_time =  buf.tm_hour*10000+buf.tm_min*100+buf.tm_sec;

                  char directory[100];
                  char buff[75];
                  sprintf(buff, "/std07095_%d_%d_%d_%d",new_job_id, getpid(),sub_date,sub_time); // puts string into buffer
                  strcpy(directory,job_list->folder_path);
                  strcat(directory, buff);

                  mkdir(directory,0777);
                  if (stat(directory, &st) == -1) {
                      perror("job: create folder");
                  } else {
                      //   change stdout and stderr
                      char file_out[120];
                      char file_err[120];
                      strcpy(file_out, directory);
                      strcpy(file_err, directory);
                      strcat(file_out, "/stdout.txt");
                      strcat(file_err, "/stderr.txt");

                      int fd_out = open(file_out, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
                      int fd_err = open(file_err, O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);

                      if(fd_out<0){
                          perror("job: create file for output");
                      }else{
                          /* change file descriptors */
                          int old_stdout = fileno(stdout);
                          dup2(fd_out, old_stdout);
                          close(fd_out);
                      }

                      if(fd_err<0){
                          perror("job: create file for error");
                      }else{
                          /* change file descriptors */
                          int old_stderr = fileno(stderr);
                          dup2(fd_err, old_stderr);
                          close(fd_err);
                      }


                  }

                  /* execute */
                  execvp(res[0],res);
                  perror("POOL execvp-> submit ");

                  /* free the memory allocated */
                  free (res);
                  exit(-1);

              }
              Job* new_job = newJob(childpid1,new_job_id);
              newJobNode(job_list,new_job);
              char to_coord[MAXBUFF]="";
              sprintf(to_coord,"Job %d %d %i",new_job_id,new_job->status,childpid1);
              WriteToPipe(job_list->jms_out,to_coord);
              kill(coord_pid,SIGUSR1);



            }

        }


    }else if (n <= 0   && errno != EAGAIN) {
        perror("console_utilities: pipe read");
        terminate=1;
    }


}



void WriteToPipe(int writefd, char* message){
    //printf("POOL   wrote to pipe\n");
    if (write(writefd, message, MAXBUFF) != MAXBUFF) {
        perror("console_utilities: data write error");
        terminate=1;
    }
}
