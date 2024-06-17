//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
using namespace std;
char previous_pwd[MAX_LINE_SIZE] = "";
int ExeCmd(std::vector<Job_class>& jobs, char* lineSize, char* cmdString)
{
    //TODO: cmd string == linesize at first, why? - for build in there is format
    // for not build in we need to think..
	char* cmd; 
	char* args[MAX_ARG];
    char pwd[MAX_LINE_SIZE] = ""; //mine
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
        if(getcwd(pwd,MAX_LINE_SIZE) != NULL){
            printf("%s/smash\n",pwd);
        }
        // #TODO: is smash appropriate name or we should do something else? + error handling
    }

    /*************************************************/
    else if (!strcmp(cmd, "cd") )
	{
        if (num_arg>1){

            strcpy(cmdString, "too many arguments");
            illegal_cmd = true;
//            printf("%s","smash error: cd: too many arguments\n");
        }
        //TODO: how to remember last location after moving out the function (save it somewhere on the comp? or in jobs)
        else if (strcmp(args[1], "-") == 0 ){
            printf("pwd is: %s\n",previous_pwd);
            if(previous_pwd[0] == NULL){
                strcpy(cmdString, "cd: OLDPWD not set");
                illegal_cmd = true;
//                printf("%s","smash error: cd: OLDPWD not set\n");
            }
            else{
                if(chdir(previous_pwd) ==-1){
                    strcpy(cmdString, "chdir: No such file or directory");
                    illegal_cmd = true;
                }
            }

        }
        else {
            char tmp_prev_pwd[MAX_LINE_SIZE];
            getcwd(tmp_prev_pwd,MAX_LINE_SIZE);
            strcpy(previous_pwd, tmp_prev_pwd);
//            printf("last location is now: %s\n",pwd);
            if(chdir(args[1]) ==-1){
                strcpy(cmdString, "chdir: No such file or directory");
                illegal_cmd = true;
            }

        }

	}
    /*************************************************/
    else if (!strcmp(cmd, "jobs"))
    {
//        std::vector<Job_class> other_jobs = create_jobs_from_other(jobs);
        sort_jobs(jobs);
        print_jobs(jobs);

    }
    /*************************************************/
    else if (!strcmp(cmd, "kill"))
    {
//        int job_id_tmp = 99;
//        int pID_tmp = 99;
//        Job_class j = Job_class(job_id_tmp, pID_tmp, cmdString, '&', "Background");
//        jobs.push_back(j);

        char* signal_arg = args[1];
        char* job_arg = args[2];
        int signal_number, job_number;

        if(signal_arg[0] == '-'){
            signal_arg[0] = ' ';
            signal_number = atoi(signal_arg);
        }
        else{
            strcpy(cmdString, "kill: invalid arguments");
            illegal_cmd = true;
        }
        if((signal_number<0) || (signal_number>32)){
            strcpy(cmdString, "kill: invalid arguments");
            illegal_cmd = true;
        }


        else {

            job_number = atoi(job_arg);
            int pid = get_pid_for_job_number(jobs, job_number);

            if (pid == -1){
                std::ostringstream oss;
                oss << "kill: job-id " << job_number << " does not exist";
                std::string err = oss.str();
                strcpy(cmdString, err.c_str());
                illegal_cmd = true;
            }
            else{
                printf("%s%d", "sending signal to process \n", pid);
            }

        }



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
 		ExeExternal(args, cmdString, num_arg, jobs);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		printf("smash error: %s\n", cmdString);
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
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg, std::vector<Job_class>& jobs)
{

    bool background_process = false;
    if (num_arg>0){
        //check if background process
        char* last_arg = args[num_arg];
        if (strcmp(last_arg, "&") == 0){
            args[num_arg] = NULL;
            num_arg--;
            background_process = true;
        }
        else if (last_arg[strlen(last_arg) - 1] == '&'){
            last_arg[strlen(last_arg) - 1] = '\0';
            background_process = true;
        }
    }

    pid_t pID;
        switch(pID = fork())
	{
    		case -1:
                // Add your code here (error)
                std::cerr << "Fork failed!" << std::endl;
                exit(1);

        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
                    //TODO: we need to check here if to split
                    printf("%s -> %s",args[0],args[1]);
                    if(execvp(args[0], args)<0){
                        cerr<<"smash error: execvp failed"<<endl;
                        exit(1);
                    }
                    std::cout << "Hello from the child process! PID: " << getpid() << std::endl;
			
			default:

                	// Add your code here
                    int job_id = next_job_id(jobs);
                    if(background_process){
                        Job_class j = Job_class(job_id, pID, cmdString, '&', "Background");
                        jobs.push_back(j);
                        printf("size of jobs now:%d\n",jobs.size());
                        return;
                    }
                    else{
                        int process_status = 0;
                        Job_class j = Job_class(job_id, pID, cmdString, '0', "Foreground");
                        waitpid(pID, &process_status, WUNTRACED);
                        j = Job_class();
                        return;
                    }


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
int BgCmd(char* lineSize, std::vector<Job_class>& jobs)
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
            exit(1);
//            return 1;
        }
        else if(childPid == 0) {
            setpgrp(); // set the process group ID of the child process
            if(execvp(args[0], args)<0){
                cerr<<"smash error: execvp failed"<<endl;
                return 1;
            }
            std::cout << "Hello from the child process! PID: " << getpid() << std::endl;


        } else {
//            int job_id = next_job_id(jobs);
//
//            Job_class j = Job_class(job_id, childPid, lineSize, '&', "Background");
//            jobs.push_back(j);
//
//            printf("size of jobs now:%d\n",jobs.size());
            return 0;
            //
//            wait(NULL);
//            sleep(5);
//            std::cout << "Hello from the parent process! PID: " << getpid() << " moving on not waiting" << std::endl;

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