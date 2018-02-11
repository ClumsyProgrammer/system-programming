#ifndef MIRRORINITIATOR_CLIENT_H
#define MIRRORINITIATOR_CLIENT_H


#include "GlobalInfo.h"


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// create client and connect to server
int new_client(int port, char *hostname);

// send string through socket
int write_to_socket(int sockfd,char* buffer);

// read string from socket
int read_from_socket(int sockfd);

// close socket
int close_client(int sockfd);

#endif //MIRRORINITIATOR_CLIENT_H
