#ifndef CONSOLE_CONSOLEUTILITITES_H
#define CONSOLE_CONSOLEUTILITITES_H

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

#define MAXBUFF 1024


extern int console_exit;

pid_t proc_find(const char* name);

void OpenCommunicationChannel(int *read_pipe, int *write_pipe, char* read_fifo, char* write_fifo);

void CloseCommunicationChannel(int readfd, int writefd);

void ReadFromPipe(int readfd);

void WriteToPipe(int writefd, char* message);



#endif //CONSOLE_CONSOLEUTILITITES_H
