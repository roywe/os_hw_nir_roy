//
// Created by USER on 7/6/2024.
//

#ifndef OS_HW_NIR_ROY_BANK_H
#define OS_HW_NIR_ROY_BANK_H
#include <map>
#include "account.h"
#include <iostream>
#include <fstream>


extern std::ofstream log;
//bank - can be an object inside atm machine or just a vector of accounts
//bank should implement - show accounts , lower account balance, add account, add_balance_to_account, delete account, move accounts
// searching account + call account command
// hash table is classic here to search for account
class Bank {

public:
    //TODO: for every action check first the bank lock and then move to specific lock
    Bank();
    bool add_account(int atm_id, int account_id, int password, int current_balance); //TODO: validate params - un negetive balance, 4 digit password, account id that not strting with 0 and 4 digits
    //TODO - also print when the account succeed
    ~Bank();
    bool change_balance(int atm_id, int account_id, int amount); //TODO - locking the account id - lock account read+write mutex + bank_lock
    bool check_password(int atm_id, int account_id, int other_password); //TODO: this action will be before all the other because found in 2 - 6 - maybe will be good to add atm_id to action..
    // also we dont have to lock in order to check
    void show_balance(int atm_id, int account_id); //TODO: need to lock account becasue there are many cases that it can change - lock account write mutex + bank_lock
    bool delete_account(int atm_id, int account_id); //TODO: need to lock account because if moving amount the it is not good - lock account write+read mutex + bank_lock
    bool move_between_accounts(int atm_id, int source_account,int target_account, int balance); // the action of the movement and print should be done atomically
    //TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)

    void lower_random_balance(); // it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts

    void print_all_accounts(); // it happened each 0.5 s  (locking all accounts) - we will need thread for this
    std::map<int, Account> bank;
//    std::map<std::int, std::vector<mutex_something> > locks; // first vector index will be reading, second for writing - we can also have the mutexes inside each account
//    mutex_something bank_lock // for lower_balance and print accounts

};

// locking points:
//TODO: if it becomes too hard for our function we can make bank just the map and implement the functions in a different file
//TODO: for locking design we can have a lock for each account existed (vector/mapping of X mutex/semaphor per accounts)
//TODO: we can also have one lock for the whole map in order to let the bank change it or show it - and it will change every time one of the accounts is locked
//page 7 - point 6 - writing to log should be somehow atomic because if A was before B then it will also appear before..

#endif //OS_HW_NIR_ROY_BANK_H
