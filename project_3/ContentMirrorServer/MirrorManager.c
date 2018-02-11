
#include "MirrorManager.h"
#include "Client.h"
#include "ProcessInput.h"

#include <stdbool.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <memory.h>
#include <sys/socket.h>

#define MAXBUFF 1024




// Require available files -> start Mirror Manager
int AskForList(Request* request){

    pthread_t tid;

//    printf("********************************\n");
//    printf("new manager thread \n");
//    printf("AskForList: received this request @ %p\n",request);
//    printRequest(request);
//    printf("********************************\n");

    pthread_create(&tid, NULL, MirrorManagerFunction, (void*)request);
    return 0;

}




// Mirror Manager functionality
void* MirrorManagerFunction(void* req){


    // https://stackoverflow.com/questions/863952/passing-structures-as-arguments-while-using-pthread-create
    Request* request = (struct Request*) req;


//    printf("********************************\n");
//    printf("Manager: received this request @ %p\n",request);
//    printRequest(request);
//    printf("Trying to connect to: %s: %d\n",request->server_address,request->server_port);
//    printf("********************************\n");


    //printf("This is a manager!\n");
    int alive=1;

//    printf("Trying to connect to: %s: %d\n",request->server_address,request->server_port);
//    printf("********************************\n");


    // new client
    int socket = new_client(request->server_port,request->server_address);

    if(socket<0){
        printf("MirrorManagerFunction:   Error: Manager cannot connect to content server \n");
        printf("MirrorManagerFunction:   Request will be ignored \n");
    }else{

        // send list request to content server
        char buffer[MAXBUFF];
        sprintf(buffer,"LIST %s:%d:%s:%d",request->server_address,request->server_port,request->content,request->delay);
        write_to_socket(socket,buffer);

        // wait for the  answer
        while(alive==1){

            // select for socket
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(socket, &rfds);

            int activity=0;
            activity = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);

            if ((activity < 0) && (errno!=EINTR)) {
                perror("MirrorManagerFunction:   MirrorManager: select");
                alive=0;
            }else{


              int i;
              for (i = 0; i < FD_SETSIZE; i++) {

                  if (FD_ISSET (i, &rfds)) {
                      if (i == socket) {
                          //printf("socket ready\n");
                          ssize_t n;
                          char buffer2[MAXBUFF];
                          bzero(buffer2,MAXBUFF);

                          n = read(socket,buffer2,MAXBUFF);
                          if (n < 0){
                              perror("MirrorManagerFunction:   ERROR reading from socket");

                          }else{

                              printf("MirrorManagerFunction:  socket content: ___%s____\n",buffer2);
                              if(strcmp(buffer2,"END")==0 ){
                                  alive=0;
                                  printf("MirrorManagerFunction:  end of communication\n");

                              }else if(strcmp(buffer2,"got your message")==0 ){
                                  ;

                              }else if(strcmp(buffer2,"This is your Mirror/Content Server")==0 ){
                                  ;

                              }else{
                                  // reform to ask request
                                  char remake[MAXBUFF];
                                  write(request->socket_of_client,buffer2,MAXBUFF);
                                  sprintf(remake,"ASK %s:%d:%s:%d",request->server_address,request->server_port,buffer2,request->delay);
                                  get_from_client(remake,request->client_address,request->client_port,request->socket_of_client);

                                  }

                              }

                          }

                      }

                  }

              }

          }
        close_client(socket);

    }

    printf("End of thread %lu, Manager is done\n",pthread_self());
    return (void *)true;
}
