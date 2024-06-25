//
// Created by USER on 6/14/2024.
//

#ifndef OS_HW_NIR_ROY_JOB_CLASS_H
#define OS_HW_NIR_ROY_JOB_CLASS_H
#include <string>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <sys/wait.h>
#include <algorithm>
#include <errno.h>
class Job_class {
    public:
        Job_class();
        Job_class(int job_id, int process_id, const std::string& command, char running_method, const std::string& status);
        Job_class(const Job_class& other);
        int get_job_id();
        int get_process_id();
        void show_job();
        ~Job_class();
        bool compare_jobs(const Job_class& other);

    public:
        int job_id;
        int process_id;
        std::string command;
        time_t create_time;
        std::string status;
        char running_method;
};

int next_job_id(std::vector<Job_class>& jobs);
void sort_jobs(std::vector<Job_class>& jobs);
void clean_jobs(std::vector<Job_class>& jobs);
void remove_jobs(std::vector<Job_class>& jobs);
void print_jobs(std::vector<Job_class>& jobs, int pid = 0);
int get_pid_for_job_number(std::vector<Job_class>& jobs, int job_id, int stopped_check = 0);
int get_max_stopped(std::vector<Job_class>& jobs);
void set_status_for_pid(std::vector<Job_class>& jobs, std::string status, int pid);
std::vector<Job_class>& create_jobs_from_other(std::vector<Job_class>& jobs);

//TODO: maybe we should also have jobs array which include the sort function (maybe it is overkill..

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

#endif //OS_HW_NIR_ROY_JOB_CLASS_H
