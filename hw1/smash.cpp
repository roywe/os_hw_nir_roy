/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include "smash.h"
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
#define MAX_LINE_SIZE 80
#define MAXARGS 20

char* L_Fg_Cmd;
int pID = -2 ;
int fg_pid = -2 ;

std::vector<Job_class> jobs;
Job_class fg_job;
char lineSize[MAX_LINE_SIZE];

int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
    struct sigaction act_INT;
    act_INT.sa_handler = &CtrlCHandler;
    act_INT.sa_flags = SA_RESTART;
    sigfillset(&(act_INT.sa_mask));
    sigaction(SIGINT, &act_INT, NULL);
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/
    struct sigaction act_TSTP;
    act_TSTP.sa_handler = &CtrlZHandler;
    act_TSTP.sa_flags = SA_RESTART;
    sigfillset(&(act_TSTP.sa_mask));
    sigaction(SIGTSTP, &act_TSTP, NULL);


	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
	    //TODO: in the file it says that build in function should ignore & but background function run first to it happened before..
    	while (1)
    	{
	 	printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);    	
		cmdString[strlen(lineSize)-1]='\0';
		ExeCmd(jobs, lineSize, cmdString);
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

