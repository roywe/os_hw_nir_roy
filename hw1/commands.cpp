//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, lineSize - command string, cmdString - at start same as linesize, after that error var
// Returns: 0 - success,1 - failure
//**************************************************************************************
using namespace std;
char previous_pwd[MAX_LINE_SIZE] = "";
int ExeCmd(std::vector<Job_class>& jobs, char* lineSize, char* cmdString)
{

	char* cmd; 
	char* args[MAX_ARG];
    char pwd[MAX_LINE_SIZE] = "";
	const char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
        if (args[i] != NULL)
			num_arg++;
 
	}
	// built -in commands ignores &
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

/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/

    //printing pid to stdout
    if (!strcmp(cmd, "showpid"))
    {
        pid_t process_id ;
        process_id = getpid();
        printf("smash pid is %d\n", process_id);
    }

    /*************************************************/
    //printing cwd to stdout
    else if (!strcmp(cmd, "pwd"))
    {
        if(getcwd(pwd,MAX_LINE_SIZE) != NULL){
            printf("%s\n",pwd); ///smash
        }
    }

    /*************************************************/
    //change directory - 2 arguments only, the first can be - (previous directory) or the location only
    // also saving previous location in previous_pwd in case there is new location
    else if (!strcmp(cmd, "cd") )
	{
    	// if no arg stay in the same dir
    	if (num_arg == 0 ){
    		return 0;
    	}
        if (num_arg>1){
            strcpy(cmdString, "cd: too many arguments");
            illegal_cmd = true;
        }
        else if (strcmp(args[1], "-") == 0 ){
            if(strcmp(previous_pwd,"")==0){
                strcpy(cmdString, "cd: OLDPWD not set");
                illegal_cmd = true;
            }
            else{
                if(chdir(previous_pwd) ==-1){
                    strcpy(cmdString, "cd: No such file or directory");
                    illegal_cmd = true;
                }
            }

        }
        else {
            char tmp_prev_pwd[MAX_LINE_SIZE];
            getcwd(tmp_prev_pwd,MAX_LINE_SIZE);
            strcpy(previous_pwd, tmp_prev_pwd);
            if(chdir(args[1]) ==-1){
                strcpy(cmdString, "cd: No such file or directory");
                illegal_cmd = true;
            }
        }

	}
    /*************************************************/
    //printing jobs sorted by job_id also cleaning irrelevant jobs before
    else if (!strcmp(cmd, "jobs"))
    {
        clean_jobs_vector(jobs);
        sort_jobs(jobs);
        print_jobs(jobs);

    }
    /*************************************************/
    //sending signal to kill, kill can get exactly 2 variables -{signal number} job-id, validating input and send signal
    else if (!strcmp(cmd, "kill"))
    {
        if ((num_arg < 2) || (num_arg > 2)){
            cerr <<"smash error: kill: invalid arguments" << endl;
            return 1;
        }

        char *signal_arg = args[1];
        char *job_arg = args[2];
        int signal_number, job_number;

        if (signal_arg[0] == '-') {
            signal_arg[0] = ' ';
            signal_number = atoi(signal_arg);
        } else {
            cerr <<"smash error: kill: invalid arguments" << endl;
            return 1;
        }

        if ((signal_number < 0) || (signal_number > 32)) {
            cerr <<"smash error: kill: invalid arguments" << endl;
            return 1;
        } else {

            job_number = atoi(job_arg);
            int pid = get_pid_for_job_number(jobs, job_number);

            if (pid == -1) {
                std::ostringstream oss;
                oss << "kill: job-id " << job_number << " does not exist";
                std::string err = oss.str();
                strcpy(cmdString, err.c_str());
                illegal_cmd = true;
            } else {
                kill(pid,signal_number);
                printf("signal number %d was sent to pid %d\n",signal_number,pid);
                return 0;
            }

        }
    }
    /*************************************************/
    // moving job to foreground and waiting for it, fg can get 1 args indicating job id, or 0 args moving the max job id
    else if (!strcmp(cmd, "fg"))
    {
        if (num_arg > 1){
            cerr << "smash error: fg: invalid arguments" << endl;
            return 1;
        }
        if (num_arg == 1){
        	if(!isPositiveInteger(args[1])){
                cerr << "smash error: fg: invalid arguments" << endl;
                return 1;
        	}
        	int jid = std::atoi(args[1]);
        	fg_job = search_and_remove_job(jobs,jid);
        	if (fg_job.job_id ==0){ //job_id is a positive num
                cerr << "smash error: fg: job-id " << jid <<" does not exist" << endl;
        		return 1;
        	}

        }

        else { // 0 args
        	if(jobs.size() ==0){
                cerr <<"smash error: fg: jobs list is empty" << endl;
                return 1;
        	}
        	fg_job = search_and_remove_job(jobs,0);

        }

		if (fg_job.status == "Stopped"){
			if(kill(fg_job.process_id,SIGCONT) == -1){
				cerr <<"smash error : kill failed" << endl;
				return 1;
			}
			fg_job.status ="Foreground";

		}
        cout << fg_job.command << " : " << fg_job.process_id << endl;
        Job_class j = Job_class(fg_job.job_id, fg_job.process_id, fg_job.command, '0', "Foreground");
        fg_job = j;
        int process_status = 0;
        waitpid(fg_job.process_id, &process_status, WUNTRACED);
        j = Job_class();

    }
    /*************************************************/
    // moving stopped job to run in background, bg can get 1 args indicating job id, or 0 args moving the max job id
    else if (!strcmp(cmd, "bg"))
    {
        if (num_arg > 1){
            cerr << "smash error: bg: invalid arguments" << endl;
            return 1;
        }
        else if (num_arg == 0){
            int max_pid = get_max_stopped(jobs); //handle max jobs
            if (max_pid == -1){
                cerr << "smash error: bg: there are no stopped jobs to resume" << endl;;
                return 1;
            }
            else{
                set_status_for_pid(jobs, "Background", max_pid);
                print_jobs(jobs, max_pid);
                kill(max_pid, SIGCONT);
                return 0;
            }
        }
        else {

            char *job_arg = args[1];
            int job_number = atoi(job_arg);
            if (job_number == 0) {
                cerr<<"smash error: bg: invalid arguments" << endl;
                return 1;
            }
            int is_job_in_jobs = get_pid_for_job_number(jobs, job_number); //check if existed
            int pid_is_stopped_job_number = get_pid_for_job_number(jobs, job_number, 1); //if existed check if stopped

            if (is_job_in_jobs == -1) {
                cerr << "smash error: bg: job-id "<< job_number <<" does not exist" << endl;

                return 1;
            } else if (pid_is_stopped_job_number == -1) {
                cerr << "smash error: bg: job-id "<< job_number <<" is already running in the background" << endl;

                return 1;
            }
            else{
                set_status_for_pid(jobs, "Background", pid_is_stopped_job_number);
                print_jobs(jobs, pid_is_stopped_job_number);
                kill(pid_is_stopped_job_number, SIGCONT);
                return 0;
            }

        }
    }
    /*************************************************/
    //  quit and quit kill - sending signals and exit small shell
    else if (!strcmp(cmd, "quit"))
    {
    	if((num_arg >0 )&& (!strcmp (args[1], "kill"))){
            if(remove_all_jobs(jobs)){ // if failed to remove jobs return 1
            	return 1;
            }
            exit(0);
    	}
    	else{
        std::vector<Job_class>().swap(jobs); // releasing memory
    	exit(0);
    	}
    }
    /*************************************************/
    //  finding if there is diff between 2 files
    else if (!strcmp(cmd, "diff"))
    {
        char* file1 = args[1];
        char* file2 = args[2];
        std::ifstream file1_content(file1);
        std::ifstream file2_content(file2);
        if (num_arg > 2){
            strcpy(cmdString, "diff: invalid arguments");
            illegal_cmd = true;

        }
        else if (!file1_content.good() || !file2_content.good()){
            strcpy(cmdString, "diff: files status is wrong- invalid arguments or file open");
            illegal_cmd = true;
        }
        else{
            string line_file1, line_file2;
            bool equal_files = true;
            while (equal_files && getline(file1_content,line_file1) && getline(file2_content,line_file2)){
                if (line_file1 != line_file2){
                    equal_files = false;
                }
            }
            if (getline(file1_content,line_file1) || getline(file2_content,line_file2)){
                equal_files = false;
            }
            file1_content.close();
            file2_content.close();
            if (equal_files){
                printf("0\n");
                return 0;
            }
            else{
                printf("1\n");
                return 1;
            }

        }

    }

	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString, num_arg, jobs,background_process);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
        cerr <<"smash error: " << cmdString << endl;
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
void ExeExternal(char *args[MAX_ARG], char* cmdString, int num_arg, std::vector<Job_class>& jobs, bool background_process)
{


    pid_t pID;
        switch(pID = fork())
	{
    		case -1:
                std::cerr << "Fork failed!" << std::endl;
                exit(1);

        	case 0 :
                	// Child Process
               		setpgrp();
                    if(execvp(args[0], args)<0){
                        cerr<<"smash error: execvp failed"<<endl;
                        exit(1);
                    }
			
			default:
                    int job_id = next_job_id(jobs);
                    if(background_process){
                        Job_class j = Job_class(job_id, pID, cmdString, '&', "Background");
                        jobs.push_back(j);
                        return;
                    }
                    else{
                        int process_status = 0;
                        Job_class j = Job_class(job_id, pID, cmdString, '0', "Foreground");
                        fg_job = j;
                        waitpid(pID, &process_status, WUNTRACED);
                        j = Job_class();
                        return;
                    }


	}
}



bool isPositiveInteger(char* str) {
    // Check if the string is empty or has a '-' at the beginning
    if ( str[0] == '-' || str[0] == '0') {
        return false;
    }

    // Check each character to ensure they are digits
    char* ptr = str;
    while (*ptr != '\0') {
        if (!std::isdigit(*ptr)) {
            return false;
        }
        ++ptr;
    }

    return true;
}
