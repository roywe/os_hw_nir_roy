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
	if (fg_job.status == "Foreground"){
		cout << "smash: caught ctrl-C,  pid "<< fg_job.process_id << endl;
		if (fg_job.process_id == 0) return;
		if (fg_job.process_id == NO_PID) {
			cout << "smash > " << flush ;
			return;
		}
		if (! kill(fg_job.process_id, SIGKILL) ) {
				cout << "smash: process " << fg_job.process_id << " was killed" << endl;
				fg_job.status = "";
	//			fg_id = NO_ID ;
	//			fg_pid = NO_PID;
	//			fg_cmd.assign("") ;
		}
		else {
			perror ("smash error : kill failed");
		}
	}
}
void CtrlZHandler(int sig_num)
{
	if (fg_job.status == "Foreground"){
		cout << "smash: caught ctrl-Z" << endl;
		if (fg_job.process_id == NO_PID) {
			cout << "smash > " << flush ;
			return;
		}

		if (! kill(fg_job.process_id, SIGSTOP) ) {
				cout << "smash: process " << fg_job.process_id << " was stopped" << endl;
				fg_job.running_method = '&';
				fg_job.status = "Stopped";

				jobs.push_back(fg_job);
				printf("size of jobs now:%d\n",jobs.size());
			fg_job.status = "";
	//			fg_id = NO_ID ;
	//			fg_pid = NO_PID;
	//			fg_cmd.assign("") ;
		}
		else {
			perror ("smash error : kill failed");
		}
	}
}
