
#ifndef CONTENTMIRRORSERVER_PROCESSINPUT_H
#define CONTENTMIRRORSERVER_PROCESSINPUT_H


// create request out of incoming string
int get_from_client(char* message, char* client, int c_port,int c_sock);


#endif //CONTENTMIRRORSERVER_PROCESSINPUT_H
