#include "jms_coord_utilities.h"
#include "jms_coord_info.h"
#include "jms_coord_update_struct.h"

void CreateCommunicationChannelToProcess(int *read_pipe, int *write_pipe, char* read_from, char* write_to){

    // create fifos
    *read_pipe = mkfifo(read_from,PERMS);
    if((*read_pipe == -1 ) && (errno != EEXIST)){
        perror("coordinator_utilities -> read pipe create");
        //ShutDown(); // exit
    }
    else{
        //printf("coordinator_utilities: READ PIPE CREATED SUCCESSFULLY\n");
        ;
    }

    *write_pipe = mkfifo(write_to,PERMS);
    if((*write_pipe == -1)  && (errno != EEXIST)){
        perror("coordinator_utilities -> write pipe create");
        //ShutDown(); // exit
    }
    else{
        //printf("coordinator_utilities: WRITE PIPE CREATED SUCCESSFULLY\n");
        ;
    }

    // open read pipe
    *read_pipe = open(read_from, O_RDONLY | O_NONBLOCK);     //Open for reading only.
    if(*read_pipe >= 0){
        //printf("coordinator_utilities: read pipe opened successfully -> fd = %d\n",*read_pipe);
        ;
    }else{
        perror("coordinator_utilities: read pipe open");
        //ShutDown(); // exit
    }

}

void OpenWritePipe(int *write_pipe, char* write_to){

    *write_pipe = open(write_to, O_WRONLY | O_NONBLOCK);     //Open for writing only.
    if(*write_pipe >= 0){
        //printf("coordinator_utilities: write pipe opened successfully -> fd = %d\n",*write_pipe);
        ;
    }else{
        perror("coordinator_utilities: write pipe open");
        //ShutDown(); // exit
    }

}

void DeleteCommunicationChannelToProcess(int readfd, int writefd, char* read_from, char* write_to){

    // close fifos
    if(close(readfd) == 0){
        //printf("coordinator_utilities: read pipe closed successfully \n");
        ;
    }else{
        perror("coordinator_utilities: read pipe close");
        //ShutDown(); // exit
    }

    if(close(writefd) == 0){
        //printf("coordinator_utilities: write pipe closed successfully \n");
        ;
    }else{
        perror("coordinator_utilities: write pipe close");
        //ShutDown(); // exit
    }

    // unlink fifos
    if(unlink(read_from) == 0 ){
        //printf("coordinator_utilities: read pipe unlinked successfully \n");
        ;
    }else{
        perror("coordinator_utilities: read pipe unlink");
        //ShutDown(); // exit
    }

    if(unlink(write_to) == 0 ){
        //printf("coordinator_utilities: write pipe unlinked successfully \n");
        ;
    }else{
        perror("coordinator_utilities: write pipe unlink");
        //ShutDown(); // exit
    }
}

int ReadFromPipe(int readfd, char* message){

    // apo diafaneies
    ssize_t n = read(readfd, message, MAXBUFF);
    if (n <= 0   && errno != EAGAIN) {
        perror("coordinator_utilities: read from pipe: ");
        //ShutDown(); // exit
    }
    return (int) n;

}

void WriteToPipe(int writefd, char* message){
    if (write(writefd, message, MAXBUFF) != MAXBUFF) {
        perror("coordinator_utilities: write to pipe: ");
        //ShutDown(); // exit
    }
}
