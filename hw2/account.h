//
// Created by USER on 7/5/2024.
//

#ifndef OS_HW_NIR_ROY_ACCOUNT_H
#define OS_HW_NIR_ROY_ACCOUNT_H
#include <pthread.h>
#include <iostream>
#include "read_write_lock.h"

class Account {
public:
    Account();
    Account(int account_id, int password, int current_balance);
//    Account(const Account& other);
    ~Account();
    int deposit(int amount);
    int withdrawn(int amount);
    //we will check.. if amount is negetive then check for negetive if amount is positive just write it
    int get_account_id();
    int get_password();
    int get_current_balance();
    void print_account() const;
    int withdrawn_by_per(float randomPer);
    bool check_password(int other_password);
    void read_lock();
    void read_unlock();
    void write_lock();
    void write_unlock();
	void lock_ww_same_order(Account& other);
	void unlock_ww_same_order(Account& other);

private:
    int account_id;
    int password;
    int current_balance;
    ReadWriteLock rw_account;
};


#endif //OS_HW_NIR_ROY_ACCOUNT_H
