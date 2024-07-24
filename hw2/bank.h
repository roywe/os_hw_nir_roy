//
// Created by USER on 7/6/2024.
//

#ifndef OS_HW_NIR_ROY_BANK_H
#define OS_HW_NIR_ROY_BANK_H
#include <map>
#include "account.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <vector>
#include <unistd.h>
#include <string>
using namespace std;
extern std::ofstream log;
//bank - can be an object inside atm machine or just a vector of accounts
//bank should implement - show accounts , lower account balance, add account, add_balance_to_account, delete account, move accounts
// searching account + call account command
// hash table is classic here to search for account
class Bank {

public:
    Bank();
    ~Bank();

    void lower_random_balance(); // it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts
	void read_lock();
	void read_unlock();
	void write_lock();
	void write_unlock();
    void print_all_accounts(); // it happened each 0.5 s  (locking all accounts) - we will need thread for this
	void write_msg_to_log(string msg);
	std::map<int, Account> accounts;
	Account bank_account;
	//mapping bank capabilities:
	//print all account - in this state nobody can write to bank, anybody can keep reading
	//lower_random_balance - in this state nobody can write or read to bank

	ReadWriteLock bank_lock;

//    std::map<std::int, std::vector<mutex_something> > locks; // first vector index will be reading, second for writing - we can also have the mutexes inside each account
//    mutex_something bank_lock // for lower_balance and print accounts

};

// locking points:

#endif //OS_HW_NIR_ROY_BANK_H
