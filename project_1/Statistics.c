
#include <malloc.h>
#include <string.h>
#include "Statistics.h"
#include "RecordManagement.h"
#include "Global.h"

void findTopDest(char* caller){

    // get list of all records for this caller
    ListOfRecords* temp;
    temp=findRecordsinHash(info->caller,caller,0,0,NULL,NULL);
    ListNode* lnode = temp->head;

    // create new max heap tree
    MaxTree* tree = newMaxTree();

    // populate tree with records
    while(lnode!=NULL){
        char* str = malloc((strlen(lnode->record->destination_number)+1)*sizeof(char));
        strcpy(str,lnode->record->destination_number);
	char* pch;
        pch = strtok(str,"-");
        newMaxNode(tree,pch,1);
        lnode = lnode->next;
        //free(str);
    }


    // print results
    if(tree->root!=NULL) {

        printMaxTreeNode(tree->root,tree->root->gain);

    }else{
        printf("No calls found\n");
    }

    // delete tree
    deleteMaxTreeN(tree);
}



void findTopCaller(int k){

    double sum_wanted = (info->earnings/100)*k;
    printf("total earnings: %f\n",info->earnings);
    printf("find sum: %f\n",sum_wanted);

// print tree by level, until sum_wanted is exhausted
    printTopCallers(sum_wanted);

}


///    http://quiz.geeksforgeeks.org/print-level-order-traversal-line-line/

/* Function to line by line print level order traversal a tree*/
void printTopCallers(double sum_wanted){
    int h = info->tree->height;
    //printf("tree height: %d\n",h);
    int i=0;
    while(i<=h+1 || sum_wanted>0){
      // print each level, top-down
        sum_wanted = printTopCallerLevel(info->tree->root,i,sum_wanted);
        i++;
    }

}

/* Print nodes at a given level */
double printTopCallerLevel(MaxNode* node, int level, double sum_wanted){

  // return sum_wanted left
    if (node == NULL)
        return 0;
    if (level == 0 && sum_wanted>0){
        double perc = (node->gain/info->earnings)*100;
        printf("caller: %s   cost: %.1f   percentage: %f %% \n",node->number,node->gain,perc);
        sum_wanted = sum_wanted - node->gain;
        return sum_wanted;

    }else if (level > 0){
        sum_wanted = printTopCallerLevel(node->right, level-1, sum_wanted);
        return printTopCallerLevel(node->left, level-1, sum_wanted);
    }else{
        return 0;
    }

}



// Piazza:
// 1) Βρίσκεις ποιοι έχουν επικοινωνήσει με τον caller1
// 2) Βρίσκεις ποιοι έχουν επικοινωνήσει με τον caller2
// 3) Βρίσκεις την τομή των 2 παραπάνω συνόλων.
// 4) Από το σύνολο που προέκυψε από το (3), αφαιρείς όποιους έχουν επικοινωνήσει μεταξύ τους.



void findSetOfEntries(char* caller1, char* caller2){

    ListOfRecords* temp1;

    // create 2 lists with all subscribers that have communicated with the given numbers

    ListOfSubs* sub1 = newSubList();
    ListOfSubs* sub2 = newSubList();

    temp1=findRecordsinHash(info->caller,caller1,0,0,NULL,NULL);
    addToListFromList(sub1,temp1,1);
    temp1=findRecordsinHash(info->caller,caller2,0,0,NULL,NULL);
    addToListFromList(sub2,temp1,1);

    temp1=findRecordsinHash(info->callee,caller1,0,0,NULL,NULL);
    addToListFromList(sub1,temp1,0);
    temp1=findRecordsinHash(info->callee,caller2,0,0,NULL,NULL);
    addToListFromList(sub2,temp1,0);

    // create the intersection

    ListOfSubs* intersection = SubListIntersection(sub1,sub2);
    if(intersection==NULL){
        printf("No common subscribers found\n");

    }else{

        // reduce further the list and print it
        Indist(intersection);
        printSubList(intersection);
        deleteSubList(intersection);


    }

    // release temp lists
    deleteSubList(sub1);
    deleteSubList(sub2);

}





////     find the subscribers that have not communicated with each other
void Indist(ListOfSubs* list){

    int flag=0;

    // if list empty -> return
    if(list->tail==NULL){
        return;
    }else if(list->tail==list->head){
      // if only one element left remove
      // two subscribers that have not communicated with each other!
        deleteSubNode(list,list->head);
        return;
    }else{

        SubNode  *current, *next, *temp;

        current = list->head;

        while(current!=NULL){
            flag=0;    // proceed normally
            if(current->next==NULL){
                break; // last node, next not available
            }

            // for every node get his contacts list

            ListOfRecords* temp1;
            ListOfSubs* sub1 = newSubList();
            temp1=findRecordsinHash(info->caller,current->subscriber,0,0,NULL,NULL);
            addToListFromList(sub1,temp1,1);
            temp1=findRecordsinHash(info->callee,current->subscriber,0,0,NULL,NULL);
            addToListFromList(sub1,temp1,0);

            next = current->next;

            // chack if any of the following nodes appears in this list
            while(next!=NULL){

                int i = containsSub(sub1,next->subscriber);
                if(i==-1){
                    flag = 1; // match found

                    temp = current->previous;
                    if(temp==NULL){
                        flag = 2;  // match found, current node was head
                    }

                    //  both nodes will be removed
                    deleteSubNode(list,current);
                    deleteSubNode(list,next);
                    break;
                }else{
                    next = next->next;
                }

            }
            // Continue according to previous state
            if(flag==0){
                current = current->next;
            }else if(flag==1){
                current = temp->next;
            }else if(flag==2){
                current = list->head;
            }
            // release temporary list
            deleteSubList(sub1);


        }

    }

}
