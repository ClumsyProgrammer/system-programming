
#ifndef CONTENTMIRRORSERVER_MIRRORMANAGER_H
#define CONTENTMIRRORSERVER_MIRRORMANAGER_H

#include "Request.h"


// Require available files -> start Mirror Manager
int AskForList(Request* request);

// Mirror Manager functionality
void* MirrorManagerFunction(void* request);


#endif //CONTENTMIRRORSERVER_MIRRORMANAGER_H
