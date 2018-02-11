#ifndef CONTENTMIRRORSERVER_UTILITIES_H
#define CONTENTMIRRORSERVER_UTILITIES_H

#include "Request.h"


// terminate
void Shutdown();

// start workers
int InitializeWorkers();

// stop workers
int TerminateWorkers();

// process request
int processRequest(Request* request);



#endif //CONTENTMIRRORSERVER_UTILITIES_H
