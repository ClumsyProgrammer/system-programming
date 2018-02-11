#ifndef JMS_COORD_CONSOLECOMMUNICATION_H
#define JMS_COORD_CONSOLECOMMUNICATION_H


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


void ReadFromConsole();
void WriteToConsole(char* message);
#endif //JMS_COORD_CONSOLECOMMUNICATION_H
