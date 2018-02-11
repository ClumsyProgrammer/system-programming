#ifndef MIRRORINITIATOR_RECEIVEINPUT_H
#define MIRRORINITIATOR_RECEIVEINPUT_H

// print incoming string to stdout
int get_from_mirror(char* message);

// send all requests to socket
int send_request_list(int sockfd);


#endif //MIRRORINITIATOR_RECEIVEINPUT_H
