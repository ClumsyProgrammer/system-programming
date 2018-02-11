#include "jms_console_utilities.h"


void OpenCommunicationChannel(int *read_pipe, int *write_pipe, char* read_fifo, char* write_fifo){

    // open pipes
    *read_pipe = open(read_fifo, O_RDONLY | O_NONBLOCK);     //Open for reading only.
    if(*read_pipe >= 0)						//Upon successful completion, open function return a non-negative integer
        printf("console_utilities: read pipe opened\n");

    else{
        //fprintf(stderr, "console_utilities: read pipe open ERROR\n");
        perror("console_utilities: read pipe open");
        console_exit=1;
    }

    *write_pipe = open(write_fifo, O_WRONLY | O_NONBLOCK);     //Open for writing only.
    if(*write_pipe >= 0)						//Upon successful completion, open function return a non-negative integer
        printf("console_utilities: write pipe opened\n");

    else{
        //fprintf(stderr, "console_utilities: write pipe open ERROR\n");
        perror("console_utilities: write pipe open");
        console_exit=1;
    }

}



void CloseCommunicationChannel(int readfd, int writefd){


    // close fifos

    if(close(readfd) == 0)
        printf("console_utilities: read pipe closed\n");

    else{
        //fprintf(stderr, "console_utilities: read pipe close ERROR\n");
        perror("console_utilities: read pipe close");
        console_exit=1;
    }

    if(close(writefd) == 0)
        printf("console_utilities: write pipe closed\n");

    else{
        //fprintf(stderr, "console_utilities: write pipe close ERROR\n");
        perror("console_utilities: write pipe close");
        console_exit=1;
    }



}


//////   SOS  char message[MAXBUFF]

void ReadFromPipe(int readfd){

    // apo diafaneies
    char message[MAXBUFF]="";
    ssize_t n;
    while((n = read(readfd, message, MAXBUFF))>0){
        message[n] = '\0';
        if(strcmp(message,"exit")==0){
            console_exit=1;
        }else{
            printf("%s\n",message);
        }
    }

    if (n <= 0   && errno != EAGAIN ) {
        perror("console_utilities: pipe read");
        console_exit=1;
    }
    return;

}



void WriteToPipe(int writefd, char* message){
    if (write(writefd, message, MAXBUFF) != MAXBUFF) {
        perror("console_utilities: data write error");
    }
}



// http://stackoverflow.com/questions/6898337/determine-programmatically-if-a-program-is-running

pid_t proc_find(const char* name)
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long  pid;
    char pname[100] = {0,};
    char state;
    FILE *fp=NULL;

    if (!(dir = opendir("/proc"))) {
        perror("can't open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL) {
        long lpid = atol(ent->d_name);
        if(lpid < 0)
            continue;
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if (fp) {
            if ( (fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3 ){
                printf("fscanf failed \n");
                fclose(fp);
                closedir(dir);
                return -1;
            }
            if (!strcmp(pname, name)) {
                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            fclose(fp);
        }
    }


    closedir(dir);
    return -1;
}
