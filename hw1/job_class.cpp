//
// Created by USER on 6/14/2024.
//

#include "job_class.h"
#include <string.h>
#include <stdio.h>

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
    std::cout << "[" << this->job_id << "]"<< " " << this->command << " : "<<this->process_id << this->running_method << " " <<seconds_elapsed<< " sec"<<status_for_repr<< std::endl;
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

bool compare_jobs(Job_class& job1,Job_class& job2){
    return job1.get_job_id() < job2.get_job_id();
}

void sort_jobs(std::vector<Job_class>& jobs){
    std::sort(jobs.begin(), jobs.end(), compare_jobs);
}
void print_jobs(std::vector<Job_class>& jobs){
    for (int i = 0; i < jobs.size(); i++) {
        jobs[i].show_job();
    }
}

std::vector<Job_class>& create_jobs_from_other(std::vector<Job_class>& jobs){
    std::vector<Job_class> other_jobs;
    for (int i = 0; i < jobs.size(); i++) {
        other_jobs[i] = jobs[i];
    }
    return other_jobs;
}

int get_pid_for_job_number(std::vector<Job_class>& jobs, int job_id){
    for (int i = 0; i < jobs.size(); i++) {
        if (jobs[i].get_job_id() == job_id){
            return jobs[i].get_process_id();
        }
    }
    return -1;
}

Job_class::Job_class(const Job_class& other){
    job_id = other.job_id;
    process_id = other.process_id;
    command = other.command;
    create_time = other.create_time;
    status = other.status;
    running_method = other.running_method;
}