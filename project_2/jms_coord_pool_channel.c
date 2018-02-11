
#include "jms_coord_pool_channel.h"
#include "jms_coord_console_channel.h"
#include "jms_coord_info.h"
#include "jms_coord_update_struct.h"


void ReadFromPool(Pool* pool){

  char message[MAXBUFF]="";

  int n=ReadFromPipe(pool->fd_in, message);

  while(n>0){
      printf("%s\n",message);
      char operation[MAXBUFF]="";
      strcpy(operation,message);
      char *pch;
      pch = strtok(operation, " ");
printf("operatioin   __%s__\n",operation);
      if(pch==NULL){
          ;
      }else if(strcmp(pch,"Job")==0 || strcmp(pch,"job")==0){

          UpdateJob(message);

      }else if(strcmp(pch,"finished")==0){
          UpdatePool(pool,1);
      }

      n = ReadFromPipe(pool->fd_in, message);


  }

return;
}
