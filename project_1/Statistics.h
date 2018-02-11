#ifndef WERHAUZ_STATISTICS_H
#define WERHAUZ_STATISTICS_H


#include "MaxHeapTree.h"
#include "Subscriber.h"

// topdest
void findTopDest(char* caller);
void findTopCaller(int k);

// top
void printTopCallers(double sum_wanted);
double printTopCallerLevel(MaxNode* node, int level, double sum_wanted);

// indist
void findSetOfEntries(char* caller1, char* caller2);
void Indist(ListOfSubs* list);

#endif //WERHAUZ_STATISTICS_H
