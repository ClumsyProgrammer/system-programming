#include <time.h>
#include "jms_coord_console_channel.h"
#include "jms_coord_info.h"
#include "jms_coord_update_struct.h"


void ReadFromConsole(){


    char message[MAXBUFF]="";

    int n=ReadFromPipe(info->console->fd_in, message);

    while(n>0){

        char operation[MAXBUFF];
        strcpy(operation,message);
        char *pch;
        pch = strtok(operation, " ");

        if(pch==NULL){
            ;
        }else if(strcmp(pch,"submit")==0){
            CreateJob(message);
        }else if(strcmp(pch,"status")==0){
            pch = strtok(NULL, " ");
            int id = atoi(pch);
            GetJobStatus(id,0,0);
        }else if(strcmp(pch,"status-all")==0){
            pch = strtok(NULL, " ");
            int laps=0;
            if(pch!=NULL){
                laps = atoi(pch); }
            GetJobStatus(0,laps,0);
        }else if(strcmp(pch,"show-active")==0){
            GetJobStatus(0,0,1);
        }else if(strcmp(pch,"show-pools")==0){
            GetPoolStatus();
        }else if(strcmp(pch,"show-finished")==0){
            GetJobStatus(0,0,2);
        }else if(strcmp(pch,"suspend")==0){
            pch = strtok(NULL, " ");
            int id = atoi(pch);
            Pool* pool = FindPoolOfJob(id);
            writeToPool(pool,message);
        }else if(strcmp(pch,"resume")==0){
            pch = strtok(NULL, " ");
            int id = atoi(pch);
            Pool* pool = FindPoolOfJob(id);
            writeToPool(pool,message);
        }else if(strcmp(pch,"shutdown")==0){
            // terminate all jms
            ShutDown();
            info->mode=0;
            printf("Begin preparations to shutdown\n");
        }else if(strcmp(pch,"exit")==0){
            unlinkClient(info->console);
        }else if(strcmp(pch,"print-status")==0){
            printClient(info->console);
            printPoolList(info->list);
        }else{
            //printf("\nUnknown command, will be discarded\n");
            ;
        }
        strcpy(message,"");

        if(info->mode!=0){
            n = ReadFromPipe(info->console->fd_in, message);
        }else{
            n=-1;
        }

    }
return;

}




void WriteToConsole(char* message){
    WriteToPipe(info->console->fd_out,message);
    printf("from coord to console:  %s\n",message );
    kill(info->console->pid,SIGUSR1);
    //printf("%s\n",message);
}
