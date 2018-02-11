
#include "Request.h"

#include <malloc.h>
#include <strings.h>


// create new empty record
Request* newEmpty() {

    Request* request = (Request *) malloc(sizeof (Request));

    bzero(request->command,10);

    bzero(request->client_address,100);
    request->client_port=-1;

    bzero(request->server_address,100);
    request->server_port=-1;

    bzero(request->content,300);
    request->delay=0;

    request->socket_of_client=-1;

    //printf("new request @ %p\n",request);

    return request;
}


// print record to stdout
void printRequest(Request *request) {

    if(request!=NULL){

        //printf("request @%p\n",request);
        printf("command: %s, from client: %s, client port: %d, server: %s, server port: %d, content: %s, delay: %d, socket of client: %d\n",
               request->command, request->client_address, request->client_port,
               request->server_address, request->server_port, request->content, request->delay, request->socket_of_client);

    }

}


// delete record, free memory
void deleteRequest(Request *request) {

    if(request!=NULL){

        free(request);

    }

}
