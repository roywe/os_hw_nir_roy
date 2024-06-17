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
std::vector<Job_class> jobs;

char lineSize[MAX_LINE_SIZE];

int main(int argc, char *argv[])
{
    char cmdString[MAX_LINE_SIZE];
	
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

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
					// perform a complicated Command
                    // start by searching string in the word, after that try to run it
//		if(!ExeComp(lineSize)) continue; //TODO what is this it isnt in the file
//					// background command
//                    // background are the jobs commands
//	 	if(!BgCmd(lineSize, jobs)) continue; //TODO: if exter is in EXE and external is background - so why we need it
					// built in commands
                    // need to understand what is the rational here because Exe is already in cmd
		ExeCmd(jobs, lineSize, cmdString);
		
		/* initialize for next line read*/
		lineSize[0]='\0';
		cmdString[0]='\0';
	}
    return 0;
}

