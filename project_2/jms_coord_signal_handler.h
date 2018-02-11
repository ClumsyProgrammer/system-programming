
#ifndef JMS_COORD_SIGNALHANDLER_H
#define JMS_COORD_SIGNALHANDLER_H


#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include "jms_coord_utilities.h"
#include "jms_coord_console_channel.h"
#include "jms_coord_info.h"



void HandleSignal(int sig, pid_t sender_pid);

Pool* findPool(pid_t pid);



#endif //JMS_COORD_SIGNALHANDLER_H
