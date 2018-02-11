#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include <sys/stat.h>
#include <unistd.h>


#include "Server.h"
#include "GlobalInfo.h"
#include "Utilities.h"

#define MAXBUFF 1024


Info* global_info;

static void multi_handler(int signo, siginfo_t *ptr, void *pVoid) {
    printf("ContentMirrorServer: Received signal with no: %d\n",signo);
}

void print_usage() {
    printf("Usage: ./ContentMirrorServer -p <port> -d <dir_of_content> -w <threadnum>\n");
}



int main(int argc, char *argv[]) {

    // block all signals
    sigset_t block_all;
    sigfillset(&block_all);
    sigprocmask (SIG_BLOCK, &block_all, NULL);

    global_info = newInfo();

    int option = 0;

    //Specifying the expected options
    while ((option = getopt(argc, argv,"p:d:w:")) != -1) {
        switch (option) {
            case 'p' :
                global_info->port = atoi(optarg);
                break;
            case 'd' :
                strcpy(global_info->dir_of_content,optarg);
                break;
            case 'w' :
                global_info->num_of_workers = atoi(optarg);
                break;
            default: print_usage();
                deleteInfo(global_info);
                exit(EXIT_FAILURE);
        }
    }



    // set sigaction
    static struct sigaction act;

    printf("set signal handling\n");
    act.sa_sigaction = multi_handler;
    sigfillset(&(act.sa_mask));

    // change signal action,
    if(sigaction(SIGINT, &act, NULL) != 0) {
        perror("ContentMirrorServer: error sigaction()");
    }
    if(sigaction(SIGHUP, &act, NULL) != 0) {
        perror("ContentMirrorServer: error sigaction()");
    }
    if(sigaction(SIGTERM, &act, NULL) != 0) {
        perror("ContentMirrorServer: error sigaction()");
    }
    if(sigaction(SIGCHLD, &act, NULL) != 0) {
        perror("ContentMirrorServer: error sigaction()");
    }



    // create path if it doesn't exist
    struct stat st = {0};
    if (stat(global_info->dir_of_content, &st) == -1) {

        pid_t childpid;
        childpid = fork();


        if (childpid == -1) {
            perror("Failed to fork");

        } else if (childpid == 0) {
            printf("create path \n");


            char *new_argv[4];
            new_argv[0] = "mkdir";
            new_argv[1] = "-p";
            new_argv[2] = global_info->dir_of_content;
            new_argv[3] = NULL;

            execvp(new_argv[0], new_argv);
            perror("execvp-> mkdir ");
            exit(-1);

        } else {
            int status;
            waitpid(childpid, &status, 0);
        }
    }


    /// create master server
    new_server(global_info->port);



    //////////////      exit     ///////////////////



    Shutdown();

    // unblock all signals
    sigprocmask (SIG_UNBLOCK, &block_all, NULL);



    return 0;


}
