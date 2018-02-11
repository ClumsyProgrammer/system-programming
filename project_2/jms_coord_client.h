#ifndef COORDINATOR_CLIENT_H
#define COORDINATOR_CLIENT_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


typedef struct Client{

    pid_t pid;

    char pipe_in[50];
    char pipe_out[50];

    int fd_in;
    int fd_out;

    int status;

    /*   STATUS
     *  Not available     -1
     *  Prepare channel    0
     *  Ready              1
     *                         */

}Client;

// create new client
Client* newClient();

// print client
void printClient(Client *console);

// create communication channel
void linkToClient(Client *console);

// open communication channel
void openToClient(Client *console);

// disconnect client
void unlinkClient(Client *console);

// delete client
void deleteClient(Client *console);


#endif //COORDINATOR_CLIENT_H
