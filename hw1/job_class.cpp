//
// Created by USER on 6/14/2024.
//

#include "job_class.h"
#include <string.h>
#include <stdio.h>

//Job_class::Job_class(){
//
//}

Job_class::Job_class(int job_id, int process_id, const std::string& command, char running_method, const std::string& status){
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

void Job_class::show_job() {
    double seconds_elapsed;
    time_t t;
    time_t time_now = time(&t);
    std::string status_for_repr = "";
    seconds_elapsed = difftime(time_now, this->create_time);
    if (this->status, "Stopped" == 0){
        status_for_repr = " (stopped)";
    }
    std::cout << "[" << this->job_id << "]"<< " " << this->command << " : "<<this->process_id << this->running_method << " " <<seconds_elapsed<< " sec"<<status_for_repr<< std::endl;
//    getpid() << std::endl;
//    printf("[%d] %s : %d%c %d secs%s",this->job_id, this->command, this->process_id, this->running_method,seconds_elapsed, status_for_repr);
}

bool Job_class::compare_jobs(const Job_class& other){
    return this->job_id > other.job_id;
}