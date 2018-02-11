
#include "ProcessInput.h"
#include "GlobalInfo.h"
#include "MirrorWorker.h"

#include <malloc.h>
#include <string.h>
#include <stdlib.h>



// create request out of incoming string
int get_from_client(char* message, char* client, int c_port,int c_sock){

    //printf("Get from client -> start:   ___%s___\n",message);
    Request* request = newEmpty();

    bzero(request->client_address,100);
    strcpy(request->client_address,client);

    request->socket_of_client=c_sock;
    request->client_port = c_port;

    char* pch;

    pch = strtok(message," ");
    if(pch==NULL){
        deleteRequest(request);
        printf("get_from_client:  Not a valid command\n");
        return -1;

    }

    // categories
    if(strcmp(pch,"GET")==0 || strcmp(pch,"FETCH")==0 || strcmp(pch,"LIST")==0 || strcmp(pch,"ASK")==0){

        bzero(request->command,10);
        strcpy(request->command,pch);

        pch = strtok(NULL,":");
        bzero(request->server_address,100);
        if(pch!=NULL)
        strcpy(request->server_address,pch);

        pch = strtok(NULL,":");
        if(pch!=NULL)
        request->server_port = atoi(pch);

        pch = strtok(NULL,":");
        bzero(request->content,300);
        if(pch!=NULL)
        strcpy(request->content,pch);

        pch = strtok(NULL,":");
        if(pch!=NULL)
        request->delay = atoi(pch);

    }else if(strcmp(message,"exit\n")==0){
        deleteRequest(request);
        printf("get_from_client:  Client will exit\n");
        return 0;

    }else if(strcmp(message,"KILL")==0){
        bzero(request->command,10);
        strcpy(request->command,"KILL");
        printf("get_from_client:  Kill thread object queued\n");

    }else{
        deleteRequest(request);
        printf("get_from_client:  Not a valid command\n");
        return -1;

    }

    //printf("New request added to list  --> ");
    //printRequest(request);
    add_request(request);
    return 0;
}
