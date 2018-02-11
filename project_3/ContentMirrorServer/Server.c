
#include <unistd.h>
#include "Server.h"
#include "GlobalInfo.h"
#include "ProcessInput.h"
#include "Utilities.h"

#define TRUE   1
#define FALSE  0

#define MAXBUFF 1024

//// http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/


/**
    Handle multiple socket connections with select and fd_set on Linux
*/

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros


int new_server(int port)
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i ,  sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[MAXBUFF];  //data buffer of 1K

    //set of socket descriptors
    fd_set readfds;

    //a message
    char *message = "This is your Mirror/Content Server ";

    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((uint16_t) port);

    //bind the socket to given port
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", port);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // start workers
    InitializeWorkers();


    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    while(global_info->mode==1)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        FD_SET(0, &readfds);  // stdin
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read R_list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

        // input from stdin
        if (FD_ISSET(0, &readfds)){
            //printf("stdin ready\n");
            char input[MAXBUFF];
            bzero(input,MAXBUFF);
            fgets(input,MAXBUFF-1,stdin); // leave space for terminating string

            if(strcmp(buffer,"exit\n")==0){
                global_info->mode=0;

            }
            break;
        }



        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            //printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //send new connection greeting message
            if( send(new_socket, message, MAXBUFF, 0) != strlen(message) )
            {
                perror("send");
            }else{
                puts("Welcome message sent successfully");
            }



            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    //printf("Adding to R_list of sockets as %d\n" , i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if (read(sd , buffer, 1024) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in R_list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }

                    //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    printf("got: %s\n",buffer);
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("From host , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    if(strcmp(buffer,"got your message")!=0){


                        get_from_client(buffer,inet_ntoa(address.sin_addr),ntohs(address.sin_port),sd);
                        bzero(buffer,MAXBUFF);
                        strcpy(buffer,"got your message");
                        send(sd , buffer , MAXBUFF , 0 );

                    }
                }
            }
        }
    }

    return 0;
}
