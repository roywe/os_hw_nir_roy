//
// Created by USER on 7/6/2024.
//

#ifndef OS_HW_NIR_ROY_ATM_MACHINE_H
#define OS_HW_NIR_ROY_ATM_MACHINE_H

#include "bank.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <vector>
#define MAX_ARG 5
#define FAIL 0
#define SUCCESS 0
#include <pthread.h>
#include <unistd.h>

using namespace std;
extern pthread_mutex_t log_mutex;

std::vector<std::string> splitString(const std::string& str, char delimiter);

extern std::string log_file;
extern std::ofstream log;

class ATM {

public :
    ATM(int atm_id, Bank * bank, std::string file_name);
    int run();// if atm failed return value is 0
    void open_account(int acc_num, int password, int balance);
    int deposit (int acc_num, int password, int amount );
    int withdraw (int acc_num, int password, int amount );
    int check_balance (int acc_num, int password);
    int close_account (int acc_num, int password);
    void transfer (int source_acc, int password,int dest_acc, int amount );
	void write_msg_to_log(string msg, bool is_error);
	bool write_log_if_no_account(int acc);
	bool write_log_if_password_not_match(int acc, int password);
//    Bank bank;
	Bank * bank;
    int atm_id;
    std::vector<std::string> commands;

};

#endif //OS_HW_NIR_ROY_ATM_MACHINE_H
