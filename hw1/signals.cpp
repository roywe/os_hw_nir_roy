// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
using namespace std;

void CtrlCHandler(int sig_num)
{
	cout << "smash: caught ctrl-C" << endl;
	if (fg_job.status == "Foreground"){

		if (fg_job.process_id == 0) return;
		if (! kill(fg_job.process_id, SIGKILL) ) {
				cout << "smash: process " << fg_job.process_id << " was killed" << endl;
				fg_job = Job_class();

		}
		else {
			cerr <<"smash error : kill failed" << endl;
		}
	}
}
void CtrlZHandler(int sig_num)
{
    //TODO: I paid attention that if we just put ctrl c in the smash things got weird
	cout << "smash: caught ctrl-Z" << endl;
	if (fg_job.status == "Foreground"){
		if (! kill(fg_job.process_id, SIGSTOP) ) {
				cout << "smash: process " << fg_job.process_id << " was stopped" << endl;
				fg_job.running_method = '&';
				fg_job.status = "Stopped";

				jobs.push_back(fg_job);

				fg_job = Job_class();
;
		}
		else {
			cerr <<"smash error : kill failed" << endl;
		}
	}
}
