#include "jms_coord_client.h"
#include "jms_coord_utilities.h"

// create new client
Client* newClient(){

    Client* console = malloc(sizeof(Client));

    console->pid    =   -1;
    console->status =   -1;
    console->fd_in  =   -1;
    console->fd_out =   -1;

    return console;
}

// print client
void printClient(Client *console){
    printf("Console connected with pid: %i, status: %d\n",
           console->pid,console->status );
    printf("fd_in: %i, fd_out: %d, pipe_in: %s, pipe_out: %s\n",
           console->fd_in,console->fd_out,console->pipe_in,console->pipe_out );
}

// create communication channel
void linkToClient(Client *console){
    CreateCommunicationChannelToProcess(&(console->fd_in),&(console->fd_out),console->pipe_in,console->pipe_out);
    kill(console->pid,SIGUSR1);
    console->status=0;
}

// open communication channel
void openToClient(Client *console){
    OpenWritePipe(&(console->fd_out),console->pipe_out);
    kill(console->pid,SIGUSR1);
    console->status=1;
}

// disconnect client
void unlinkClient(Client *console){
    DeleteCommunicationChannelToProcess(console->fd_in, console->fd_out, console->pipe_in, console->pipe_out);
    console->status=-1;
}



// delete client
void deleteClient(Client *console){
    free(console);
}
