// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#define NO_PID -2
using namespace std;

void CtrlCHandler(int sig_num)
{
	pid_t j_pid = getpid();
	cout << "smash: caught ctrl-C, pid : "<< j_pid << " job pid "<< fg_job.process_id << endl;
	if (fg_job.process_id == 0) return;
	if (j_pid == NO_PID) {
		cout << "smash > " << flush ;
		return;
	}
	if (! kill(fg_job.process_id, SIGKILL) ) {
			cout << "smash: process " << j_pid << " was killed" << endl;
//			fg_id = NO_ID ;
//			fg_pid = NO_PID;
//			fg_cmd.assign("") ;
	}
	else {
		perror ("smash error : kill failed");
	}
}
void CtrlZHandler(int sig_num)
{
	pid_t j_pid = getpid();
	cout << "smash: caught ctrl-Z" << endl;
	if (j_pid == NO_PID) {
		cout << "smash > " << flush ;
		return;
	}

	if (! kill(fg_job.process_id, SIGSTOP) ) {
			cout << "smash: process " << fg_job.process_id << " was stopped" << endl;
			fg_job.running_method = '&';
			fg_job.status = "Stopped";

			jobs.push_back(fg_job);
			printf("size of jobs now:%d\n",jobs.size());
//			fg_id = NO_ID ;
//			fg_pid = NO_PID;
//			fg_cmd.assign("") ;
	}
	else {
		perror ("smash error : kill failed");
	}

}
