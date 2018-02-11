
#include "ProcessInput.h"
#include "GlobalInfo.h"
#include "Client.h"


int get_from_mirror(char* message){
    printf("socket content: %s\n",message);
    if(strcmp(message,"exit")==0){
        global_info->mode=0;
    }
    return 0;
}


int send_request_list(int sockfd){

    // send all queued requests

    if(global_info->list->tail==NULL){
        printf("List empty\n");
    }else{
        RequestNode* node;
        node = global_info->list->head;
        while(node!=NULL){
            write_to_socket(sockfd,node->request);
            node = node->next;
        }
    }

    return  0;

}
