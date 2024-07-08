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
using namespace std;

extern std::string log_file;

class ATM {

public :
    ATM(int atm_id, Bank bank, std::string file_name);
    int run();// if atm failed return value is 0
    void open_account(int acc_num, int password, int balance);
    void deposit (int acc_num, int password, int amount );
    void withdraw (int acc_num, int password, int amount );
    void check_balance (int acc_num, int password);
    void close_account (int acc_num, int password);
    void transfer (int source_acc, int password,int dest_acc, int amount );
    Bank bank;
    int atm_id;
    std::vector<std::string> commands;

};


#endif //OS_HW_NIR_ROY_ATM_MACHINE_H
