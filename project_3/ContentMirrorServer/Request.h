
#ifndef CONTENTMIRRORSERVER_REQUEST_H
#define CONTENTMIRRORSERVER_REQUEST_H



typedef struct Request{

    char command[10];  // GET, FETCH, LIST, ASK
    char client_address[100];
    int client_port;
    int socket_of_client;

    char server_address[100];
    int server_port;
    char content[300];
    int delay;

}Request;



// create new empty request
Request* newEmpty();
// print request to stdout
void printRequest(Request *request);
// delete request, free memory
void deleteRequest(Request *request);





#endif //CONTENTMIRRORSERVER_REQUEST_H
