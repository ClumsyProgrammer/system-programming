
#include "jms_coord_signal_handler.h"
#include "jms_coord_pool_channel.h"
#include "jms_coord_update_struct.h"




////    while( ( pid = waitpid( -1, &signal, WNOHANG ) ) > 0 )





void HandleSignal(int sig, pid_t sender_pid){

    Pool* pool;

    if(info->console->pid == sender_pid){
        if(sig == SIGUSR1){
            if(info->console->status==-1){
                linkToClient(info->console);
            }else if(info->console->status==0){
                openToClient(info->console);
            }else{
                ReadFromConsole();
            }
        }else if(sig == SIGINT){
            printf("Console will exit\n");
            unlinkClient(info->console);
        }

    }else if((pool=findPool(sender_pid))!=NULL){
        if(sig == SIGUSR1){
            if(pool->status==0){
                ReadFromPool(pool);
            }
          }
    }else{
        if(sig == SIGUSR1){
            // new console
            info->console->pid = sender_pid;
            linkToClient(info->console);
        }else if(sig==SIGTERM){
            info->mode=0; // shutdown
            ShutDown();
        }else if(sig == SIGINT){
            info->mode=0; // shutdown
            ShutDown();
        }
    }



}





Pool* findPool(pid_t pid){

    if(info!=NULL && info->list!=NULL){
      if(info->list->tail==NULL){
          return NULL;
      }else{
          PoolNode* node;
          node = info->list->head;
          while(node!=NULL){
              if(node->pool->pid == pid){
                  return node->pool;
              }
              node = node->next;
          }
      }
    }

    return NULL;
}
