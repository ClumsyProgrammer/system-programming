#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include "jms_pool_utilities.h"


#define FIFO1   "./fifo_1"
#define FIFO2   "./fifo_2"
#define PATH    "./results"


int terminate = 0;
JobList* job_list;
pid_t coord_pid;



static void multi_handler(int sig, siginfo_t *siginfo, void *context) {

//////    while( ( pid = waitpid( -1, &signal, WNOHANG ) ) > 0 )

    if(sig == SIGTERM) {
        printf("jms_pool: SIGTERM -> exit\n");
        terminate = 1;
        return;
    } else if(sig == SIGINT) {
        printf("jms_pool: SIGINT -> ignore\n");
        return;
    // }else if(sig == SIGCHLD) {
    //     //printf("jms_pool: SIGCLD from %d -> ok\n",siginfo->si_pid);
    //     Job* job = findJobWithPID(siginfo->si_pid);
    //     job->status = 2;
    //     job_list->jobs_active--;
    //     char to_coord[MAXBUFF]="";
    //     sprintf(to_coord,"Job %d %i",job->id,job->status);
    //     WriteToPipe(job_list->jms_out,to_coord);
    //     kill(coord_pid,SIGUSR1);
    //     if(job_list->max_jobs==job_list->jobs_served){
    //       if(job_list->jobs_active==0){
    //         terminate=1;
    //       }
    //     }
    //     return;
    } else if(sig == SIGUSR1) {
        coord_pid = siginfo->si_pid;
        ReadFromPipe(job_list->jms_in);

        return;
    }
    return;
}
// https://linuxprograms.wordpress.com/2012/06/22/c-getopt-example/

void print_usage() {
    printf("Usage:./jms_pool -n <max_jobs> -p <path> \n");
}

int main(int argc, char *argv[]) {

    ///  block all signals until ready

//printf("this is the pool executing\n");
    // https://www.gnu.org/software/libc/manual/html_node/Testing-for-Delivery.html#Testing-for-Delivery
    sigset_t block_all;
    sigfillset(&block_all);
    sigprocmask (SIG_BLOCK, &block_all, NULL);

    job_list = newJobList();


    ////    process arguments


    int option = 0;
    char* folder_path=malloc(sizeof(char)*LINE_MAX);
    char* jms_in = malloc(sizeof(char)*LINE_MAX);
    char* jms_out = malloc(sizeof(char)*LINE_MAX);

    // default
    strcpy(jms_in,FIFO2);
    strcpy(jms_out,FIFO1);
    char buff[20];
    sprintf(buff, "_%d", getpid()); // puts string into buffer
    strcat(jms_in, buff);
    strcat(jms_out, buff);
    strcpy(folder_path,PATH);




    while ((option = getopt(argc, argv,"n:p:r:w:")) != -1) {
        switch (option) {
            case 'n' :
                job_list->max_jobs = atoi(optarg);
                break;
            case 'p' :
                strcpy(folder_path,optarg);
                break;
            case 'r' :
                strcpy(jms_in,optarg);
                break;
            case 'w' :
                strcpy(jms_out,optarg);
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }


    job_list->folder_path = folder_path;

    /////////   initialize
    static struct sigaction act;
    // prepare sigaction
    act.sa_sigaction = &multi_handler;
    //////   http://stackoverflow.com/questions/8400530/how-can-i-tell-in-linux-which-process-sent-my-process-a-signal
    act.sa_flags |= SA_SIGINFO; // get detail info
    sigfillset(&(act.sa_mask));


    // change signal action,
    if(sigaction(SIGINT, &act, NULL) != 0) {
        perror("jms_pool: error sigaction()");
    }
    if(sigaction(SIGUSR1, &act, NULL) != 0) {
        perror("jms_pool: error sigaction()");
    }
    if(sigaction(SIGTERM, &act, NULL) != 0) {
        perror("jms_pool: error sigaction()");
    }
    if(sigaction(SIGCHLD, &act, NULL) != 0) {
        perror("jms_pool: error sigaction()");
    }

    // open fifos
    OpenCommunicationChannel(&(job_list->jms_in),&(job_list->jms_out),jms_in,jms_out);
    WriteToPipe(job_list->jms_out,"ready");



    ////  ready ->  unblock all signals
    sigprocmask (SIG_UNBLOCK, &block_all, NULL);


    ////  wait for signals or all children to exit





//////////   http://stackoverflow.com/questions/21762208/notify-parent-process-when-child-process-dies

    while (terminate==0){


              siginfo_t exit_info;
              int retVal = waitid(P_ALL, -1, &exit_info, WEXITED);

              if (retVal == -1 && errno == ECHILD && job_list->max_jobs==job_list->jobs_served){
                printf("No more children\n");
                terminate=1;
                break;
              }

              if(retVal!=-1){

                printf("Child with pid %i finished\n",exit_info.si_pid);
                Job* job = findJobWithPID(exit_info.si_pid);
                if(job!=NULL){


                  job->status = 2;
                  job_list->jobs_active--;
                  char to_coord[MAXBUFF]="";
                  sprintf(to_coord,"Job %d %i",job->id,job->status);
                  WriteToPipe(job_list->jms_out,to_coord);
                  kill(coord_pid,SIGUSR1);
                  if(job_list->max_jobs==job_list->jobs_served){
                    if(job_list->jobs_active==0){
                      terminate=1;
                    }
                  }

                }

              }


        sleep(1);
    }


    ///   exit

    printf("jms_pool: EXIT\n");

    WriteToPipe(job_list->jms_out,"finished");
    kill(coord_pid,SIGUSR1);


    CloseCommunicationChannel(job_list->jms_in,job_list->jms_out);

    deleteJobList(job_list);

    free(folder_path);
    free(jms_in);
    free(jms_out);


    return 0;
}
