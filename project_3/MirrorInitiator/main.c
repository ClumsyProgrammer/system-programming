#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <errno.h>

#include "GlobalInfo.h"
#include "Client.h"
#include "ProcessInput.h"


Info* global_info;

static void multi_handler(int signo, siginfo_t *ptr, void *pVoid) {
    printf("Received signal with no: %d\n",signo);
}

void print_usage() {
    printf("Usage: ./MirrorInitiator -n <MirrorServerAddress> -p <MirrorServerPort> \\\\\n"
                   "-s <ContentServerAddress1:ContentServerPort1:dirorfile1:delay1, \\\\\n"
                   "ContentServerAddress2:ContentServerPort2:dirorfile2:delay2, ...>    \n");
}



int main(int argc, char *argv[]) {

    // block all signals
    sigset_t block_all;
    sigfillset(&block_all);
    sigprocmask (SIG_BLOCK, &block_all, NULL);

    int option = 0;
    global_info = newInfo();

    //Specifying the expected options
    while ((option = getopt(argc, argv,"n:p:s:")) != -1) {
        switch (option) {
            case 'n' :
                strcpy(global_info->mirror_address,optarg);
                break;
            case 'p' :
                global_info->mirror_port = atoi(optarg);
                break;
            case 's' :
                // https://stackoverflow.com/questions/3939157/c-getopt-multiple-value
                optind--;
                for( ;optind < argc && *argv[optind] != '-'; optind++){
                    newNode(global_info->list,argv[optind]);
                }
                break;
            default: print_usage();
                deleteInfo(global_info);
                exit(EXIT_FAILURE);
        }
    }

    printList(global_info->list);

    // set sigaction
    static struct sigaction act;

    printf("set signal handling\n");
    act.sa_sigaction = multi_handler;
    sigfillset(&(act.sa_mask));

    // change signal action,
    if(sigaction(SIGINT, &act, NULL) != 0) {
        perror("MirrorInitiator: error sigaction()");
    }
    if(sigaction(SIGHUP, &act, NULL) != 0) {
        perror("MirrorInitiator: error sigaction()");
    }
    if(sigaction(SIGTERM, &act, NULL) != 0) {
        perror("MirrorInitiator: error sigaction()");
    }
    if(sigaction(SIGCHLD, &act, NULL) != 0) {
        perror("MirrorInitiator: error sigaction()");
    }

    // unblock all signals
    sigprocmask (SIG_UNBLOCK, &block_all, NULL);


    ///  create client
    int socket = new_client(global_info->mirror_port,"localhost");

    //printf("new socket @ %d\n",socket);

    if(socket<0){
        // exit
        perror("MirrorInitiator: create socket error");
        printf("MirrorInitiator: exit\n");
        deleteInfo(global_info);
        return 0;
    }else {

        send_request_list(socket);

        printf("\nProvide input:\n");          // get input from stdin
        while (global_info->mode > 0) {


            // use select for stdin and socket

            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            FD_SET(socket, &rfds);

            int activity=0;
            activity = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

            if ((activity < 0) && (errno!=EINTR)) {
                perror("select");
                exit(EXIT_FAILURE);
            }

            int i;
            for (i = 0; i < FD_SETSIZE; i++) {

                if (FD_ISSET (i, &rfds)) {
                    if (i == 0) {
                        //printf("stdin ready\n");
                        char buffer[MAXBUFF-4]; // leave space for GET
                        bzero(buffer,MAXBUFF-4);
                        fgets(buffer,MAXBUFF-5,stdin); // leave space for terminating string

                        if(strcmp(buffer,"exit\n")==0){
                            global_info->mode=0;
                            write_to_socket(socket,buffer);

                        }else{

                          char to_send[MAXBUFF];
                          bzero(to_send,MAXBUFF);
                          sprintf(to_send,"GET %s",buffer);
                          write_to_socket(socket,to_send);

                        }



                    } else if (i == socket) {
                        //printf("socket ready\n");
                        read_from_socket(socket);


                    }

                }

            }

        }

    }



    //////////////      exit     ///////////////////

    printf("MirrorInitiator: exit\n");
    close_client(socket);
    deleteInfo(global_info);


    return 0;


}
