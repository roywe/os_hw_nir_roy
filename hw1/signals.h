#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <ctime>
#include <string>
#include "smash.h"

void CtrlCHandler(int sig_num);
void CtrlZHandler(int sig_num);





#endif

