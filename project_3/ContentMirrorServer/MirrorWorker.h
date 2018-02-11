
#ifndef CONTENTMIRRORSERVER_MIRRORWORKER_H
#define CONTENTMIRRORSERVER_MIRRORWORKER_H

#include <stdbool.h>
#include "Request.h"
#include "GlobalInfo.h"


// unlock mutex upon exit
void cleanup_unlock_mutex(void *p);

// Worker functionality
void *WorkerFunction();

// add request to list
bool add_request(Request* request);

// extract request from list
Request* remove_request();

// request file from content server
int ask_file(Request* request);



#endif //CONTENTMIRRORSERVER_MIRRORWORKER_H
