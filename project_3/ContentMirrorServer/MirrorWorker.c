
#include "MirrorWorker.h"
#include "Utilities.h"
#include "Client.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <wait.h>

#define MAXBUFF 1024


// mutex & condition valiable
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


// unlock mutex upon exit
void cleanup_unlock_mutex(void *p)
{
    pthread_mutex_unlock(p);
}


// Worker functionality
void* WorkerFunction(){
    //printf("This is a worker!\n");
    int alive = 1;

    // https://stackoverflow.com/questions/11939422/safe-cancellation-of-pthreads-using-a-cleanup-handler-in-c
    /* Install a cleanup handler */
    pthread_mutex_lock(&mtx);
    pthread_cleanup_push( cleanup_unlock_mutex, &mtx);


    while(alive==1){

        // wait for request to get queued
        pthread_cond_wait(&cond,&mtx);
        // extract request
        Request* request=remove_request();
        // unlock mutex
        pthread_mutex_unlock(&mtx);

        // Process request
        //printf("WorkerFunction:    request to extract @ %p\n",request);
        if(request!=NULL){
            //printf("WorkerFunction:    thread %lu extracted the following request\n",pthread_self());
            //printf("-->");
            //printRequest(request);
            if(strcmp(request->command,"KILL")==0){
                alive=0; // terminate
            }else{
                processRequest(request);
            }

        }

    }

    /* Uninstall cleanup handler */
    pthread_cleanup_pop(1);

    printf("WorkerFunction:    End of thread %lu\n",pthread_self());
    return (void *)true;
}


// add request to list
bool add_request(Request* request){

    // lock mutex
    pthread_mutex_lock(&mtx);
    // add request to list
    newRNode(global_info->R_list,request);
    // broadcast update of list with condition variable
    pthread_cond_broadcast(&cond);
    // unlock mutex
    pthread_mutex_unlock(&mtx);

    return true;

}


// extract request from list
Request* remove_request(){

    Request* request = extractRNode(global_info->R_list,global_info->R_list->head);

    return request;
}


// request file from content server
int ask_file(Request* request){

    //printf("Ask file-start by thread %lu\n",pthread_self());

    // create path if it doesn't exist
    struct stat st = {0};

    // directory name to save remote content
    char remote_dirorfile[400];
    bzero(remote_dirorfile,400);
    strcat(remote_dirorfile,global_info->dir_of_content);
    strcat(remote_dirorfile,"/");
    strcat(remote_dirorfile,request->server_address);
    strcat(remote_dirorfile,"/");
    char port[10];
    sprintf(port,"%d",request->server_port);
    strcat(remote_dirorfile,port);
    strcat(remote_dirorfile,"/");

    // remote hierarchy of folders
    char content[300];
    strcpy(content,request->content);
    char *pch1, *pch2;
    pch1= strtok(content,"/");
    pch2= strtok(NULL,"/");

    while(pch2!=NULL && pch1!=NULL){

        strcat(remote_dirorfile,pch1);
        strcat(remote_dirorfile,"/");
        pch1 = pch2;
        pch2= strtok(NULL,"/");

    }

    printf("dir to create: %s\n",remote_dirorfile);

    // create directory
    if (stat(remote_dirorfile, &st) == -1) {

        pid_t childpid;
        childpid = fork();

        if (childpid == -1) {
            perror("ask_file:    Failed to fork");

        } else if (childpid == 0) {
            printf("ask_file:    create path \n");


            char *new_argv[4];
            new_argv[0] = "mkdir";
            new_argv[1] = "-p";
            new_argv[2] = remote_dirorfile;
            new_argv[3] = NULL;

            execvp(new_argv[0], new_argv);
            perror("execvp-> mkdir ");
            exit(-1);

        } else {
            int status;
            waitpid(childpid, &status, 0);
        }
    }


    // add filename
    if(pch1!=NULL)
    strcat(remote_dirorfile,pch1);
    printf("filename: %s\n",remote_dirorfile);

    // prepare file
    FILE *received_file;
    // progress
    int complete=0;
    int bytes_received=0;

    //printf("Trying to connect to: %s: %d\n",request->server_address,request->server_port);
    //printf("********************************\n");

    // new client
    int socket = new_client(request->server_port,request->server_address);
    if(socket<0){
        printf("ask_file:    Error connecting with content server\n");
        printf("ask_file:    Request will be ignored\n");
    }else {

        // open local file
        received_file = fopen(remote_dirorfile, "w");
        if (received_file == NULL)
        {
            fprintf(stderr, "ask_file:    Failed to open file foo --> %s\n", strerror(errno));
            close_client(socket);
            return -1;

        }

        // send request to content server
        char buffer[MAXBUFF];
        sprintf(buffer, "FETCH %s:%d:%s:%d", request->server_address, request->server_port, request->content,
                request->delay);
        write_to_socket(socket, buffer);
        //printf("ask for file:  sent to socket: %s\n",buffer);

        // wait for answer
        while (complete == 0) {

            // select for socket
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(socket, &rfds);

            int activity = 0;
            activity = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

            // in case of error abort
            if ((activity < 0) && (errno != EINTR)) {
                perror("ask_file:    MirrorWorker: select - ask for file");
                close_client(socket);
                fclose(received_file);
                return -1;
            }

            // when socket ready process input
            int i;
            for (i = 0; i < FD_SETSIZE; i++) {

                if (FD_ISSET (i, &rfds)) {
                    if (i == socket) {
                        //printf("ask_file:    socket ready\n");
                        ssize_t n;
                        char buffer2[MAXBUFF];
                        bzero(buffer2, MAXBUFF);

                        n = read(socket, buffer2, MAXBUFF);
                        if (n < 0) {
                            // ignore and continue
                            perror("ask_file:    ERROR reading from socket");

                        } else {

                            //printf("ask_file:         file trans: socket content: ___%s____\n", buffer2);

                            if (strcmp(buffer2, "END") == 0) {
                                complete = 1;
                                printf("ask_file:    receive file:   end of communication\n");

                            } else if (strcmp(buffer2, "got your message") == 0) {
                                // ignore
                                ;

                            } else if (strcmp(buffer2, "This is your Mirror/Content Server") == 0) {
                                // ignore
                                ;

                            } else {

                                ///   save file
                                printf("ask_file:    Received file content: %s\n",buffer2);
                                fwrite(buffer2, sizeof(char), (size_t) n, received_file);
                                bzero(buffer2,MAXBUFF);
                                bytes_received = (int) (bytes_received + n);
                                printf("ask_file:    bytes received: %d\n",bytes_received);

                            }

                        }

                    }

                }

            }

        }
        printf("Going to close file and socket\n");
        fclose(received_file);
        close_client(socket);

    }

    return 0;
    //printf("--> ask file finished by thread %lu\n",pthread_self());
    
}
