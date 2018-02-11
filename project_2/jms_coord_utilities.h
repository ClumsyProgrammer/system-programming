
#ifndef JMS_COORD_COORDINATORUTILITIES_H
#define JMS_COORD_COORDINATORUTILITIES_H



#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <time.h>



#define MAXBUFF 1024
#define PERMS   0666


void CreateCommunicationChannelToProcess(int *read_pipe, int *write_pipe, char* filename1, char* write_to);

void OpenWritePipe(int *write_pipe, char* write_to);

void DeleteCommunicationChannelToProcess(int readfd, int writefd, char* read_from, char* write_to);

int ReadFromPipe(int readfd, char* message);

void WriteToPipe(int writefd, char* message);






#endif //JMS_COORD_COORDINATORUTILITIES_H
