
#ifndef JMS_COORD_INFORMSTRUCT_H
#define JMS_COORD_INFORMSTRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <wait.h>
#include <time.h>
#include "jms_coord_pool.h"


#define MAXBUFF 1024
#define PERMS   0666


void CreateJob(char* input);
Pool* CreatePool();

void GetJobStatus(int id, int lapsed, int status);
void GetPoolStatus();

Pool* FindPoolToInsertJob();
Pool* FindPoolOfJob(int id);

Job* FindJob(int id);

void UpdatePool(Pool* pool,int status);
void UpdateJob(char* message);

void ShutDown();

#endif //JMS_COORD_INFORMSTRUCT_H
