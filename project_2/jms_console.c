#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>

#include "jms_console_utilities.h"

#define MAXBUFF 1024
#define FIFO1   "./fifo_1"
#define FIFO2   "./fifo_2"

static struct sigaction act;

char* jms_in = FIFO2;
char* jms_out = FIFO1;
char* input = NULL;

int readfd;
int writefd;

int coordinator_up = 0;
int console_exit=0;



static void multi_handler(int sig, siginfo_t *siginfo, void *context) {
    // get pid of sender,
    pid_t sender_pid = siginfo->si_pid;

    if(sig == SIGINT){
        printf("jms_console: SIGINT from [%d] will be ignored\n",  (int)sender_pid);
        return;
    }else if(sig == SIGUSR1){
        //printf("jms_console: SIGUSR1, from [%d]\n", (int)sender_pid);
        if(coordinator_up==0){
            coordinator_up=1;
            printf("jms_console: coordinator has created channels\n");
            OpenCommunicationChannel(&readfd,&writefd,jms_in,jms_out);
            kill(sender_pid, SIGUSR1);
        }else if(coordinator_up==1){
            coordinator_up=2;
            printf("jms_console: coordinator has opened write pipe\n");

            // read from operation file if given
            if(input!=NULL){

                FILE *stream = NULL;
                char *line=malloc(sizeof(char)*LINE_MAX);
                size_t len = 0;
                ssize_t read;
                stream = fopen(input,"r");
                //printf("operation file open: %p\n",stream);

                if(stream!=NULL){
                    // read file line by line
                    read = getline(&line, &len, stream);
                    while (read != -1){
                        line[read - 1] = '\0';  // remove \n character
                        int ok=0,i=0;
                        while (line[i] != '\0') {
                            if (!isspace(line[i]))
                                ok=1;
                            i++;
                        }
                        if(ok==1){   ///   no white line
                            WriteToPipe(writefd,line);
                        }
                        read = getline(&line, &len, stream); // read next line
                    }
                    fclose(stream);
                    kill(sender_pid, SIGUSR1);  // inform coordinator fifo has data

                }else{
                    printf("jms_console: Error opening file: %s\n",input);
                }
                free(line);
            }

        }else{
            ReadFromPipe(readfd);
        }


        return;
    }

    return;
}
// https://linuxprograms.wordpress.com/2012/06/22/c-getopt-example/
void print_usage() {
    printf("Usage: ./jms jms_console -w <jms in> -r <jms out> -o <operations file>\n");
}

int main(int argc, char *argv[]) {

    ///  block all signals until ready


    // https://www.gnu.org/software/libc/manual/html_node/Testing-for-Delivery.html#Testing-for-Delivery
    sigset_t block_all;
    sigfillset(&block_all);
    sigprocmask (SIG_BLOCK, &block_all, NULL);

    ////    process arguments

    int option = 0;
    char* jms_in_pipe=malloc(sizeof(char)*LINE_MAX);
    char* jms_out_pipe=malloc(sizeof(char)*LINE_MAX);
    char* operations_file=malloc(sizeof(char)*LINE_MAX);


    while ((option = getopt(argc, argv,"w:r:o:")) != -1) {
        switch (option) {
            case 'w' :
                strcpy(jms_out_pipe,optarg);
                jms_out = jms_out_pipe;
                break;
            case 'r' :
                strcpy(jms_in_pipe,optarg);
                jms_in = jms_in_pipe;
                break;
            case 'o' :
                strcpy(operations_file,optarg);
                input = operations_file;
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }


    /////////   initialize

    // prepare sigaction
    act.sa_sigaction = &multi_handler;
    //////   http://stackoverflow.com/questions/8400530/how-can-i-tell-in-linux-which-process-sent-my-process-a-signal
    act.sa_flags |= SA_SIGINFO; // get detail info
    sigfillset(&(act.sa_mask));


    // change signal action,
    if(sigaction(SIGINT, &act, NULL) != 0) {
        perror("jms_console: error sigaction()");
    }
    if(sigaction(SIGUSR1, &act, NULL) != 0) {
        perror("jms_console: error sigaction()");
    }


    // is server running?
    pid_t pid = proc_find("jms_coord");
    if (pid == -1) {
        printf("jms_console: jms_coord: not found\n");
        free(jms_in_pipe);
        free(jms_out_pipe);
        free(operations_file);
        printf("jms_console: bye bye from jms_console\n");
        return 0;

    } else {
        printf("jms_console: jms_coord: %d\n", pid);

        kill(pid, SIGUSR1);
    }


    ////  ready ->  unblock all signals
    sigprocmask (SIG_UNBLOCK, &block_all, NULL);
    ////  wait for input to send to pipe

    int k=0;
    while(coordinator_up<2&&k<6){
        printf("jms_console: waiting for coordinator\n");
        sleep(1);
        k++;
    }

    if(coordinator_up<2){
      printf("jms_console: coordinator not responding\n");
    }else{
      printf("\nProvide input:\n");
      while(console_exit!=1){
          char* str = malloc(LINE_MAX);
          fgets (str, LINE_MAX, stdin);
          if(str==NULL){
              printf("Please provide valid input:\n");
          }
          char* op;
          op = strtok(str,"\n");

          if(op!=NULL){

              int is_same = (strcmp(op,"exit")*strcmp(op,"shutdown"));
              if (is_same==0){
                  console_exit=1;
              }
              WriteToPipe(writefd,op);
              kill(pid, SIGUSR1);

          }
          free(str);
      }
    }




///////////       exit          /////////////////
    printf("jms_console: exit\n");

    WriteToPipe(writefd,"exit");
    kill(pid, SIGUSR1);
    CloseCommunicationChannel(readfd,writefd);

    free(jms_in_pipe);
    free(jms_out_pipe);
    free(operations_file);

    return 0;
}
