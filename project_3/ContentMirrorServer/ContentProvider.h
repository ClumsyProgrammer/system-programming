
#include "Request.h"

#ifndef CONTENTMIRRORSERVER_CONTENTPROVIDER_H
#define CONTENTMIRRORSERVER_CONTENTPROVIDER_H

// send back list of available files that much the request
int return_list(Request* request);

// send back requested file
int fetch_file(Request* request);

#endif //CONTENTMIRRORSERVER_CONTENTPROVIDER_H
