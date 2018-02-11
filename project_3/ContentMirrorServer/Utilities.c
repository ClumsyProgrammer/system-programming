
#include "Utilities.h"
#include "GlobalInfo.h"
#include "MirrorWorker.h"
#include "ProcessInput.h"
#include "MirrorManager.h"
#include "ContentProvider.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFF 1024


// terminate
void Shutdown(){

    printf("ContentMirrorServer: exit\n");
    printInfo(global_info);
    // stop active threads
    TerminateWorkers();
    printInfo(global_info);
    // free global info
    deleteInfo(global_info);

}



// start workers
int InitializeWorkers(){

    printf("Initializing Workers\n");
    // array of tids
    pthread_t tid[global_info->num_of_workers];

    // create threads and add them to the thread list
    int i;
    for(i=0; i<global_info->num_of_workers; i++){

        //printf("new thread %d\n",i+1);
        pthread_create(&tid[i], NULL, WorkerFunction, NULL);
        newTNode(global_info->T_list,tid[i]);

    }

    return  0;
}




// stop workers
int TerminateWorkers(){

    // add to list kill request

    if(global_info->T_list->tail==NULL){
        printf("List empty\n");
        return -1;
    }else{
        ThreadNode* node;
        node = global_info->T_list->head;
        while(node!=NULL){

            // queue kill object
            char command[10];
            strcpy(command,"KILL");
            get_from_client(command,"dummy",0,-2);

            node = node->next;
        }
    }


    // join finished thread and remove it from list

    if(global_info->T_list->tail==NULL){
        printf("List empty\n");
        return -1;
    }else{
        ThreadNode* node;
        node = global_info->T_list->head;
        while(node!=NULL){

            pthread_join(node->tid,NULL);
            //printf("thread with id %lu exited\n",node->tid);
            ThreadNode* temp_node;
            temp_node = node;
            node = node->next;
            deleteTNode(global_info->T_list,temp_node);
        }
    }


    printf("All done -- exiting\n");
    return 0;


}



// process request
int processRequest(Request* request){

    // categorize request and invoque proper function
    // when finished free request

    if(strcmp(request->command,"GET")==0){
        printf("Must split GET request\n");
        AskForList(request);
        deleteRequest(request);

    }else if(strcmp(request->command,"LIST")==0){
        printf("Let me show you what I ve got\n");
        return_list(request);
        deleteRequest(request);

    }else if(strcmp(request->command,"FETCH")==0){
        printf("Let me send you what I ve got\n");
        fetch_file(request);
        deleteRequest(request);

    }else if(strcmp(request->command,"ASK")==0){
        printf("Let me ask for what you want\n");
        ask_file(request);
        deleteRequest(request);

    }else{
        printf("Unknown command\n");
        deleteRequest(request);

    }

    return  0;
}
