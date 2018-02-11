#ifndef MIRRORCONTENT_CLIENT_H
#define MIRRORCONTENT_CLIENT_H


// create client and connect to server
int new_client(int port, char *hostname);

// send string through socket
int write_to_socket(int sockfd,char* buffer);

// read string from socket
int read_from_socket(int sockfd);

// close socket
int close_client(int sockfd);


#endif //MIRRORCONTENT_CLIENT_H
