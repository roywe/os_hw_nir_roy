//
// Created by USER on 6/16/2024.
//

#ifndef OS_HW_NIR_ROY_SMASH_H
#define OS_HW_NIR_ROY_SMASH_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20

extern char* L_Fg_Cmd;
extern std::vector<Job_class> jobs;

extern char lineSize[MAX_LINE_SIZE];
#endif //OS_HW_NIR_ROY_SMASH_H
