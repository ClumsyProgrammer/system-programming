#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include "jms_coord_info.h"
#include "jms_coord_signal_handler.h"
#include "jms_coord_update_struct.h"

#define FIFO1   "./fifo_1"
#define FIFO2   "./fifo_2"
#define PATH    "./results"

static void multi_handler(int sig, siginfo_t *siginfo, void* context) {
    HandleSignal(sig,siginfo->si_pid);
}

void print_usage() {
    printf("Usage: ./jms_coord -l <path> -n <jobs pool> -w <jms out> -r <jms in>\n");
}

// global struct
Info* info;

// https://linuxprograms.wordpress.com/2012/06/22/c-getopt-example/
int main(int argc, char *argv[]) {

    // block all signals
    // https://www.gnu.org/software/libc/manual/html_node/Testing-for-Delivery.html#Testing-for-Delivery
    sigset_t block_all;
    sigfillset(&block_all);
    sigprocmask (SIG_BLOCK, &block_all, NULL);

    int option = 0;
    char* folder_path=malloc(sizeof(char)*LINE_MAX);
    char* jms_in_pipe=malloc(sizeof(char)*LINE_MAX);
    char* jms_out_pipe=malloc(sizeof(char)*LINE_MAX);
    // default
    strcpy(folder_path,PATH);
    strcpy(jms_in_pipe,FIFO1);
    strcpy(jms_out_pipe,FIFO2);

    info = newInfo();

    //Specifying the expected options
    while ((option = getopt(argc, argv,"l:n:w:r:")) != -1) {
        switch (option) {
            case 'l' :
                strcpy(folder_path,optarg);
                break;
            case 'n' :
                info->pool_size=atoi(optarg);
                break;
            case 'w' :
                strcpy(jms_out_pipe,optarg);
                break;
            case 'r' :
                strcpy(jms_in_pipe,optarg);
                break;
            default: print_usage();
                exit(EXIT_FAILURE);
        }
    }


    strcpy(info->console->pipe_in,jms_in_pipe);
    strcpy(info->console->pipe_out,jms_out_pipe);
    strcpy(info->path,folder_path);

    free(folder_path);
    free(jms_in_pipe);
    free(jms_out_pipe);

    // create path if it doesn't exist
    struct stat st = {0};
    if (stat(info->path, &st) == -1) {

        pid_t childpid2;
        childpid2 = fork();


        if (childpid2 == -1) {
            perror("Failed to fork");

        } else if (childpid2 == 0) {
            printf("create path \n");


            char *new_argv[4];
            new_argv[0] = "mkdir";
            new_argv[1] = "-p";
            new_argv[2] = info->path;
            new_argv[3] = NULL;

            execvp(new_argv[0], new_argv);
            perror("execvp-> mkdir ");
            exit(-1);

        } else {
            int status;
            waitpid(childpid2, &status, 0);
        }
    }





    // set sigaction
    static struct sigaction act;

    printf("set signal handling\n");
    act.sa_sigaction = &multi_handler;
    //////   http://stackoverflow.com/questions/8400530/how-can-i-tell-in-linux-which-process-sent-my-process-a-signal
    act.sa_flags |= SA_SIGINFO; // get detail info
    sigfillset(&(act.sa_mask));

    // change signal action,
    if(sigaction(SIGINT, &act, NULL) != 0) {
        perror("jms_coord: error sigaction()");
    }
    if(sigaction(SIGUSR1, &act, NULL) != 0) {
        perror("jms_coord: error sigaction()");
    }
    if(sigaction(SIGTERM, &act, NULL) != 0) {
        perror("jms_coord: error sigaction()");
    }
    if(sigaction(SIGCHLD, &act, NULL) != 0) {
        perror("jms_coord: error sigaction()");
    }

    // unblock all signals
    sigprocmask (SIG_UNBLOCK, &block_all, NULL);

    /////////////    wait for signals    /////////////
    printf("jms_coord: Waiting for signals \n");

    /* wait until signaled to exit */
    while(info->mode==1){
        printf("jms_coord: Sleeping \n");
        sleep(3);


    }


    //////////////      exit     ///////////////////

    printf("jms_coord: exit\n");

    deleteInfo(info);

    return 0;


}
