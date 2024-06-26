//
// Created by USER on 6/14/2024.
//

#include "job_class.h"
#include <string.h>
#include <stdio.h>
using namespace std;

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

}

//getters
int Job_class::get_job_id(){
    return this->job_id;
}

int Job_class::get_process_id(){
    return this->process_id;
}

//show job as in the convention of the exercise
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
}

bool Job_class::compare_jobs(const Job_class& other){
    return this->job_id > other.job_id;
}

//for jobs vector find new max id
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
// removes all jobs from jobs vector, at first send SIGTEM, if doesnt work send SIGKILL
int remove_all_jobs(std::vector<Job_class>& jobs){
    for (std::vector<Job_class>::iterator it = jobs.begin(); it != jobs.end(); ) {
        if ( kill(it->process_id, SIGTERM) == -1 ) {  // send SIGTERM signal
           	cerr <<"smash error: kill failed" << endl;
           	return 1;
        }
          	cout << "[" << it->job_id << "] " << it->command << " - Sending SIGTERM... " << flush ;
          	sleep(5);
        	pid_t result = waitpid(it->process_id, NULL, WNOHANG);
         	if (result == -1) {
         		cerr <<"smash error: waitpid failed" << endl;
         		return 1;
          	}
			else if (result ) // if the process was terminated the remove from list and go to the next job
			{
				cout << "Done." << endl;
				it = jobs.erase(it);
				continue;
			}
			if ( kill(it->process_id, SIGKILL) == -1 ) { // if process was not terminated send signal SIGKILL
				cerr << "smash error: kill failed" << endl;
				return 1;
			}
			it = jobs.erase(it);
			cout << "(5 sec passed) Sending SIGKILL... Done." << endl;
    }
    return 0;
}
// removes jobs which have finished from the jobs vector
void clean_jobs_vector(std::vector<Job_class>& jobs){
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

// Function to search and remove job by job id,
// if job id is zero removes the highest job id from jobs vector
Job_class search_and_remove_job(std::vector<Job_class>& jobs, int job_id) {

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
            Job_class found_job = *it;
            jobs.erase(it);
            return found_job;
    	}
    }


    return Job_class();
}

//compare between jobs for the sort function
bool compare_jobs(Job_class& job1,Job_class& job2){
    return job1.get_job_id() < job2.get_job_id();
}

void sort_jobs(std::vector<Job_class>& jobs){
    std::sort(jobs.begin(), jobs.end(), compare_jobs);
}
void print_jobs(std::vector<Job_class>& jobs, int pid){
    for (size_t i = 0; i < jobs.size(); i++) {
        if (pid == 0){
            jobs[i].show_job();
        }
        else{
            if (jobs[i].process_id == pid){
                std::cout << jobs[i].command << " : "<<jobs[i].process_id << std::endl;
            }
        }

    }
}

//find pid for job_id in the jobs vector, status option for stopped is available
int get_pid_for_job_number(std::vector<Job_class>& jobs, int job_id, int stopped_check){
    for (size_t i = 0; i < jobs.size(); i++) {
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

//setting status for pid
void set_status_for_pid(std::vector<Job_class>& jobs, std::string status, int pid){
    for (size_t i = 0; i < jobs.size(); i++) {
        int current_pid = jobs[i].get_process_id();
        if (current_pid == pid){
            jobs[i].status = status;
        }
    }
}

//get maximum job that stopped returning its pid
int get_max_stopped(std::vector<Job_class>& jobs){
    int max_pid = -1;
    for (size_t i = 0; i < jobs.size(); i++) {
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
