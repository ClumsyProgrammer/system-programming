
#include "ContentProvider.h"

#include "GlobalInfo.h"

#include "Client.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


#define MAXBUFF 1024


// send back list of available files that much the request
int return_list(Request* request){

    FILE *fp;
    int status;
    char path[PATH_MAX];
    char dir_path[PATH_MAX];
    sprintf(dir_path,"find ./%s/%s -type f",global_info->dir_of_content,request->content);

    fp = popen(dir_path, "r");
    if (fp == NULL){
        printf("return_list:  Error accessing directory\n");
        return -1;
    }

    while (fgets(path, PATH_MAX, fp) != NULL){
        //printf("%s", path);
        char* right_path;
        right_path = strtok(path,"\n");
        //printf("right path: ___%s___\n",right_path);
        write_to_socket(request->socket_of_client,right_path);
        bzero(path,PATH_MAX);
    }

    // signal end of communication
    bzero(path,PATH_MAX);
    strcpy(path,"END");
    write_to_socket(request->socket_of_client,path);

    status = pclose(fp);
    return status;

}


// send back requested file
int fetch_file(Request* request){


    printf("fetch_file request:\n");
    printRequest(request);

    // https://stackoverflow.com/questions/2014033/send-and-receive-a-file-in-socket-programming-in-linux-with-c-c-gcc-g

    int bytes_sent=0;

    printf("fetch_file:   Lets open %s\n",request->content);

    FILE* source_file = NULL;
    source_file = fopen(request->content,"r");
    char buffer[MAXBUFF];
    bzero(buffer,MAXBUFF);

    if(source_file!=NULL){

        printf("fetch_file:    file to send is open \n");

        size_t br = fread(buffer, MAXBUFF, 1, source_file);
        while(br>0){

            printf("fetch_file:    send part of file: %s\n",buffer);
            ssize_t bs = write(request->socket_of_client, buffer, MAXBUFF);
            bzero(buffer,MAXBUFF);
            br = fread(buffer, MAXBUFF, sizeof(char), source_file);
            bytes_sent = (int) (bytes_sent + bs);
        }

        ///  end of file met
        fclose(source_file);
        bzero(buffer,MAXBUFF);
        strcpy(buffer,"END");
        write_to_socket(request->socket_of_client, buffer);
        printf("fetch_file:   Bytes sent = %d \n",bytes_sent);

    }else{

        // error opening file
        fprintf(stderr, "fetch_file:      Error opening file --> %s \n", strerror(errno));
        bzero(buffer,MAXBUFF);
        strcpy(buffer,"END");
        write(request->socket_of_client, buffer, MAXBUFF);
    }

    return 0;

}
