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
            printf("%s\n",pwd); ///smash
        }
        // #TODO: is smash appropriate name or we should do something else? + error handling
    }

    /*************************************************/
    else if (!strcmp(cmd, "cd") )
	{
        if (num_arg>1){

            strcpy(cmdString, "cd: too many arguments");
            illegal_cmd = true;
//            printf("%s","smash error: cd: too many arguments\n");
        }
        //TODO: how to remember last location after moving out the function (save it somewhere on the comp? or in jobs)
        else if (strcmp(args[1], "-") == 0 ){
//            printf("pwd is: %s\n",previous_pwd);
            if(previous_pwd[0] == NULL){
                strcpy(cmdString, "cd: OLDPWD not set");
                illegal_cmd = true;
//                printf("%s","smash error: cd: OLDPWD not set\n");
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
//            printf("last location is now: %s\n",pwd);
            if(chdir(args[1]) ==-1){
                strcpy(cmdString, "cd: No such file or directory");
                illegal_cmd = true;
            }

        }

	}
    /*************************************************/
    else if (!strcmp(cmd, "jobs"))
    {
    	clean_jobs(jobs);
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
        if ((num_arg < 2) || (num_arg > 2)){
            printf("smash error: %s\n", "kill: invalid arguments");
            return 1;
        }

        char *signal_arg = args[1];
        char *job_arg = args[2];
        int signal_number, job_number;

        if (signal_arg[0] == '-') {
            signal_arg[0] = ' ';
            signal_number = atoi(signal_arg);
        } else {
            printf("smash error: %s\n", "kill: invalid arguments");
            return 1;
        }

        if ((signal_number < 0) || (signal_number > 32)) {
            printf("smash error: %s\n", "kill: invalid arguments");
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
        	fg_job = searchAndRemoveJob(jobs,jid);
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
        	fg_job = searchAndRemoveJob(jobs,0);

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
    else if (!strcmp(cmd, "bg"))
    {
        if (num_arg > 1){
            cerr << "smash error: bg: invalid arguments" << endl;
            return 1;
        }
        else if (num_arg == 0){
            int max_pid = get_max_stopped(jobs);
            if (max_pid == -1){
                cerr << "smash error: bg: there are no stopped jobs to resume" << endl;;
                return 1;
            }
            else{
                set_status_for_pid(jobs, "Background", max_pid);
                print_jobs(jobs, max_pid);
                kill(max_pid, SIGCONT);
            }
        }
        else {

            char *job_arg = args[1];
            int job_number = atoi(job_arg);
            if (job_number == 0) {
                cerr<<"smash error: bg: invalid arguments" << endl;
                return 1;
            }
            int is_job_in_jobs = get_pid_for_job_number(jobs, job_number);
            int pid_is_stopped_job_number = get_pid_for_job_number(jobs, job_number, 1);

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
            }

        }
    }
    /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {

    	if((num_arg >0 )&& (!strcmp (args[1], "kill"))){
    	remove_jobs(jobs);

    	}
    	else{
        std::vector<Job_class>().swap(jobs); // releasing memory
    	exit(0);
    	}
    }
    /*************************************************/
    else if (!strcmp(cmd, "diff"))
    {
        size_t maxSize;
        bool are_files_equal = true;
        char* file1 = args[1];
        char* file2 = args[2];
//        std::cout << "file1 : " << file1 << "file2 : " << file2 << std::endl;
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
                    if(execvp(args[0], args)<0){
                        cerr<<"smash error: execvp failed"<<endl;
                        exit(1);
                    }
//                    std::cout << "Hello from the child process! PID: " << getpid() << std::endl;
			
			default:

                	// Add your code here
                    int job_id = next_job_id(jobs);
                    if(background_process){
                        Job_class j = Job_class(job_id, pID, cmdString, '&', "Background");
                        jobs.push_back(j);
//                        printf("size of jobs now:%d\n",jobs.size());
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




//std::vector<std::string> readFile(std::string myFile){
//    std::vector<std::string> log;
//    std::string buffer;
//    std::string b;
//    std::ifstream input_file(myFile);
//    int o = 1;
//    if (!input_file.is_open())
//    {
//        std::cout << "Could not open the file " << myFile << std::endl;
////        exit(EXIT_FAILURE);
//        return log;
//    }
//
//    while (input_file >> buffer)
//    {
//        if (o > 2)
//        {
//            log.push_back(b);
//            o = 0;
//            b.erase();
//        }
//        else {
//            b.append(buffer);
//            b.append(" ");
//            ++o;
//        }
//    }
//
//    input_file.close();
//    return log;
//}

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
