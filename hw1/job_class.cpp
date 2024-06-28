//
// Created by USER on 6/14/2024.
//

#include "job_class.h"
#include <string.h>
#include <stdio.h>
using namespace std;
//Job_class::Job_class(){
//
//}

Job_class::Job_class(){
    time_t t = time(0);
    this->process_id = 0;
    this->job_id = 0;
    this->create_time = time(&t);
    this->status = "";
    this->running_method = '0';
    this->command = "";
}

Job_class::Job_class(int job_id, int process_id, const std::string& command, const char running_method, const std::string& status){
    time_t t;
    this->process_id = process_id;
    this->job_id = job_id;
    this->create_time = time(&t);
    this->status = status;
    this->running_method = running_method;
    this->command = command;

}

Job_class::~Job_class(){
//    delete this->command;
//    delete this->create_time;
//    delete this->status;
}

int Job_class::get_job_id(){
    return this->job_id;
}

int Job_class::get_process_id(){
    return this->process_id;
}

void Job_class::show_job() {
    double seconds_elapsed;
    time_t t;
    time_t time_now = time(&t);
    std::string status_for_repr = "";
    seconds_elapsed = difftime(time_now, this->create_time);
    if (this->status == "Stopped"){
        status_for_repr = " (stopped)";
    }
    std::cout << "[" << this->job_id << "]"<< " " << this->command << " : "<<this->process_id << " " <<seconds_elapsed<< " secs"<<status_for_repr<< std::endl;
//    getpid() << std::endl;
//    printf("[%d] %s : %d%c %d secs%s",this->job_id, this->command, this->process_id, this->running_method,seconds_elapsed, status_for_repr);
}

bool Job_class::compare_jobs(const Job_class& other){
    return this->job_id > other.job_id;
}



int next_job_id(std::vector<Job_class>& jobs){
    int max_job_id = 1;
    int size = jobs.size();
    if (size == 0){
        return max_job_id;
    }
    else {
        for (int i = 0; i < size; i++) {
            if (max_job_id < jobs[i].get_job_id()){
                max_job_id = jobs[i].get_job_id();
            }
        }
        return max_job_id+1;
    }

}

void remove_jobs(std::vector<Job_class>& jobs){
    for (std::vector<Job_class>::iterator it = jobs.begin(); it != jobs.end(); ) {
        if ( kill(it->process_id, SIGTERM) == -1 ) {  // send SIGTERM signal
           	perror("smash error: kill failed");
//        	return FAILURE;
        }
          	cout << "[" << it->job_id << "] " << it->command << " - Sending SIGTERM... " << flush ;
          	sleep(5);
        	pid_t result = waitpid(it->process_id, NULL, WNOHANG);
         	if (result == -1) {
          		if ( errno != ESRCH) {
         			perror("smash error: waitpid failed");
//           			return FAILURE;
        		}
          	}
			else if (result ) // if the process was terminated the remove from list and go to the next job
			{
				cout << "Done." << endl;
				it = jobs.erase(it);
				continue;
			}
			if ( kill(it->process_id, SIGKILL) == -1 ) { // if process was not terminated send signal SIGKILL
				perror("smash error: kill failed");
//				return FAILURE;
			}
			it = jobs.erase(it);
			cout << "(5 sec passed) Sending SIGKILL... Done." << endl;
    }
    exit(0);
}

void clean_jobs(std::vector<Job_class>& jobs){
    for (std::vector<Job_class>::iterator it = jobs.begin(); it != jobs.end(); ) {
    	pid_t result = waitpid(it->process_id, NULL, WNOHANG);

        if (result){
        	it = jobs.erase(it);
        }
        else {
        	++it;
        }
    }
}

// Function to search and remove job by job_id
Job_class searchAndRemoveJob(std::vector<Job_class>& jobs, int job_id) {
    // if job_id zero find max job id
	if (job_id == 0) {
        for (std::vector<Job_class>::iterator it = jobs.begin(); it != jobs.end(); ++it) {
            if (it->job_id > job_id) {
            	job_id = it->job_id;
            }
        }

    }
        // Iterate through the vector to find the job with the given job_id
    for (std::vector<Job_class>::iterator it = jobs.begin(); it != jobs.end(); ++it) {
    	if (it->job_id == job_id) {
                // Store the job to return
            Job_class found_job = *it;
                // Remove the job from the vector
            jobs.erase(it);
                // Return the found job
            return found_job;
    	}
    }


    return Job_class();
}

bool compare_jobs(Job_class& job1,Job_class& job2){
    return job1.get_job_id() < job2.get_job_id();
}

void sort_jobs(std::vector<Job_class>& jobs){
    std::sort(jobs.begin(), jobs.end(), compare_jobs);
}
void print_jobs(std::vector<Job_class>& jobs, int pid){
    for (int i = 0; i < jobs.size(); i++) {
        if (pid == 0){
            jobs[i].show_job();
        }
        else{
            if (jobs[i].process_id == pid){
                std::cout << jobs[i].command << " : "<<jobs[i].process_id << std::endl;
                //" "<< jobs[i].running_method <<
            }
        }

    }
}

std::vector<Job_class>& create_jobs_from_other(std::vector<Job_class>& jobs){
    std::vector<Job_class> other_jobs;
    for (int i = 0; i < jobs.size(); i++) {
        other_jobs[i] = jobs[i];
    }
    return other_jobs;
}

int get_pid_for_job_number(std::vector<Job_class>& jobs, int job_id, int stopped_check){
    for (int i = 0; i < jobs.size(); i++) {
        if (jobs[i].get_job_id() == job_id){
            if (stopped_check == 0){
                return jobs[i].get_process_id();
            }
            else{
                if (jobs[i].status == "Stopped"){
                    return jobs[i].get_process_id();
                }
                else{
                    return -1;
                }
            }
        }
    }
    return -1;
}

void set_status_for_pid(std::vector<Job_class>& jobs, std::string status, int pid){
    for (int i = 0; i < jobs.size(); i++) {
        int current_pid = jobs[i].get_process_id();
        if (current_pid == pid){
            jobs[i].status = status;
        }
    }
}

int get_max_stopped(std::vector<Job_class>& jobs){
    int max_pid = -1;
    for (int i = 0; i < jobs.size(); i++) {
        int current_pid = jobs[i].get_process_id();
        if (jobs[i].status == "Stopped" && max_pid<current_pid){
            max_pid = current_pid;
        }
    }
    return max_pid ;
}

Job_class::Job_class(const Job_class& other){
    job_id = other.job_id;
    process_id = other.process_id;
    command = other.command;
    create_time = other.create_time;
    status = other.status;
    running_method = other.running_method;
}
