#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include "job_class.h"
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <ctype.h>
#include <sstream>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, std::vector<Job_class>& jobs);
int ExeCmd(std::vector<Job_class>& jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg, std::vector<Job_class>& jobs);
#endif

