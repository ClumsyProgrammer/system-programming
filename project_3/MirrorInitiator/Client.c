#include <errno.h>
#include <fcntl.h>
#include "Client.h"
#include "ProcessInput.h"
#define MAXBUFF 1024

// create client and connect to server
int new_client(int port, char *hostname)
{
    int sockfd;

    uint16_t portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = (uint16_t) port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // open socket
    if (sockfd < 0){
        perror("ERROR opening socket");
        return -1;
    }

    // get server's address (fill struct)
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          (size_t) server->h_length);
    serv_addr.sin_port = htons(portno);

    // connect to server
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        return -1;
    }

    // if all successfull return socket's fd
    return sockfd;
}



// send string through socket
int write_to_socket(int sockfd,char* buffer){
    ssize_t n;

    if(strcmp(buffer,"exit")==0){
        global_info->mode=0;
    }else{
        n = write(sockfd,buffer,MAXBUFF);
        if (n < 0){
            perror("ERROR writing to socket");
            return -1 ;
        }
    }

    return 0;
}



// read string from socket
int read_from_socket(int sockfd){
    ssize_t n;
    char buffer[MAXBUFF];
    bzero(buffer,MAXBUFF);

    n = read(sockfd,buffer,MAXBUFF);
    if (n < 0){
        perror("ERROR reading from socket");
        return -1;
    }

    if(n>0){
        get_from_mirror(buffer);
    }else{
        //Mirror disconnected
        printf("Host disconnected \n");
        // exit
        global_info->mode=0;
    }

    return  0;

}

// close socket
int close_client(int sockfd){

    if((close(sockfd))<0){
        perror("ERROR closing socket");
        return -1;
    }
    return 0;
}
