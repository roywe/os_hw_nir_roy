//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(std::vector<Job_class> jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE] = {'\0'}; //mine
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
//        printf("%s,",args[i]); //mine
        if (args[i] != NULL)
			num_arg++;
 
	}
//    printf("%s",pwd);
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/

    if (!strcmp(cmd, "showpid"))
    {
        pid_t process_id;
        process_id = getpid();
        printf("smash pid is %d\n", process_id);
        // #TODO: is \n fine or not + error handling
    }

    /*************************************************/
    else if (!strcmp(cmd, "pwd"))
    {
//        char pwd_for_pwd_cmd[MAX_LINE_SIZE];
        getcwd(pwd,MAX_LINE_SIZE);
        printf("%s/smash\n",pwd);
        // #TODO: is smash appropriate name or we should do something else? + error handling
    }

    /*************************************************/
    else if (!strcmp(cmd, "cd") )
	{
        //TODO: 0 args / invalid new location
//        if (num_arg == 0){
//            printf("wrong params amount\n");
//        }
        if (num_arg>1){

            strcpy(cmdString, "too many arguments");
            illegal_cmd = true;
//            printf("%s","smash error: cd: too many arguments\n");
        }
        //TODO: how to remember last location after moving out the function (save it somewhere on the comp? or in jobs)
        else if (strcmp(args[1], "-") == 0 ){
            printf("pwd is: %s\n",pwd);
            if(pwd[0] == NULL){
                strcpy(cmdString, "cd: OLDPWD not set");
                illegal_cmd = true;
//                printf("%s","smash error: cd: OLDPWD not set\n");
            }
            else{
                chdir(pwd);
            }

        }
        else {

            char tmp_prev_pwd[MAX_LINE_SIZE];
            getcwd(tmp_prev_pwd,MAX_LINE_SIZE);
            strcpy(pwd, tmp_prev_pwd);
//            printf("last location is now: %s\n",pwd);
            chdir(args[1]);
        }

	}
    /*************************************************/
    else if (!strcmp(cmd, "jobs"))
    {

        int size = jobs.size();

        for (int i = 0; i < size; i++) {
            printf("%s\n", "entered jobs");
            jobs[i].show_job();
        }
    }
    /*************************************************/
    else if (!strcmp(cmd, "kill"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "fg"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "bg"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "diff"))
    {

    }
    /*************************************************/
    //TODO: there is no mkdir in the file
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}

	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
			default:
                	// Add your code here
					printf("");
					/*

					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, std::vector<Job_class> jobs)
{
	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];

//    printf("linesize is - %s %c",lineSize, lineSize[strlen(lineSize)-2]);//mine
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)

        pid_t childPid = fork();

        if (childPid < 0) {
            // fork() failed
            std::cerr << "Fork failed!" << std::endl;
            return 1;
        }
        else if(childPid == 0) {
            std::cout << "Hello from the child process! PID: " << getpid() << std::endl;

            int job_id = 1;
//            std::string status = "Background";
//            Job_class* j = new Job_class(job_id, getpid(), lineSize, '&', "Background");
            jobs.push_back(Job_class(job_id, getpid(), lineSize, '&', "Background"));
            jobs[0].show_job();
        } else {
//
//            wait(NULL);
//            sleep(5);
            std::cout << "Hello from the parent process! PID: " << getpid() << " moving on not waiting" << std::endl;

        }


    }
	return -1;
}

//struct job(){
//    int/string job_id
//    string command
//    int process id
//    time -> time of entering the array
//    status - stopped/ running etc
//
//    sort() - sort by jobs (without changing list) - before run remove job (if status is wrong)
//    free() - when killed we need to free job space
//
//}
//jobs - need to save the jobs that are out somehow


//            j->show_job();
//            execv(lineSize);
//            struct sigaction sa = {.sa_handler = sigintHandler};
//            sigfillset(&sa.sa_mask);
//            sigaction(SIGINT, &sa, NULL);
//            while(1) {
//                if(receivedSignal) { exit(0); }
//                sleep(1);
//            }

// Wait for the child process to complete
//            wait(NULL); //backgroud - not waiting
//            sleep(1);
//            kill(childPid, SIGINT);