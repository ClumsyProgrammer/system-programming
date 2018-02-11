#ifndef JMS_POOL_POOLUTILITIES_H
#define JMS_POOL_POOLUTILITIES_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include "jms_pool_job_list.h"

#define MAXBUFF 1024


extern int terminate;
extern pid_t coord_pid;
extern JobList* job_list;


void OpenCommunicationChannel(int *read_pipe, int *write_pipe, char* filename1, char* filename2);

void CloseCommunicationChannel(int readfd, int writefd);

void ReadFromPipe(int readfd);

void WriteToPipe(int writefd, char* message);


#endif //JMS_POOL_POOLUTILITIES_H
