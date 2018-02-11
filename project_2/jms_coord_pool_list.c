#include "jms_coord_pool_list.h"


// create list of pools
PoolList* newPoolList(){
    PoolList* list = malloc(sizeof(PoolList));
    //printf("new list @ %p\n",list);
    list->head = NULL;
    list->tail = NULL;

    return list;
}
PoolNode* newPoolNode(PoolList* list, Pool* pool){
    PoolNode* node = malloc(sizeof(PoolNode));
    //printf("new node @ %p\n",node);
    if(list!=NULL && pool!=NULL){
      if(list->tail==NULL){   // if list is empty -> new root
          list->head = node;
          list->tail = node;
          node->next = NULL;
          node->previous = NULL;
          node->pool = pool;
      }else{
          list->tail->next = node;
          node->previous = list->tail;
          list->tail = node;
          node->next = NULL;
          node->pool = pool;
      }
    }else{
      free(node);
      node=NULL;
    }

    return node;
}

// print list of pools
void printPoolList(PoolList* list){
  if(list!=NULL){
    if(list->tail==NULL){
        printf("List empty\n");
    }else{
        PoolNode* node;
        node = list->head;
        while(node!=NULL){
            printPoolNode(node);
            node = node->next;
        }
    }
  }
}
void printPoolNode(PoolNode* node){
    printPool(node->pool);
}

// delete list of pools
void deletePoolList(PoolList* list){
  if(list!=NULL){
    if(list->tail==NULL){
        //printf("List empty\n");
        free(list);
    }else{
        PoolNode *current, *next;
        current = list->head;
        while(current!=NULL){
            next = current->next;
            deletePoolNode(list,current);
            current=next;
        }
        list->head = NULL;
        list->tail = NULL;
        free(list);
    }
  }

    //printf("List deleted\n");
}

void deletePoolNode(PoolList* list,PoolNode* node){
  if(list!=NULL && node!=NULL){
    if(node->next==NULL){               // delete tail
        list->tail = node->previous;
    }
    if(node->previous==NULL){           // delete head
        list->head = node->next;
        if(node->next!=NULL){
            node->next->previous=NULL;
        }
    }else{                              // delete middle node
        PoolNode* previous = node->previous;
        PoolNode* next = node->next;
        if(next!=NULL)
            next->previous=previous;
        previous->next=next;
    }
    deletePool(node->pool);
    free(node);
  }

}
