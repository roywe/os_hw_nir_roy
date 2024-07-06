//
// Created by USER on 7/5/2024.
//

#ifndef OS_HW_NIR_ROY_ACCOUNT_H
#define OS_HW_NIR_ROY_ACCOUNT_H
#include <pthread.h>

class Account {
public:
    Account();
    Account(int account_id, int password, int current_balance);
    Account(const Account& other);
    ~Account();
    bool change_balance(int amount);//TODO:amount will add to balance - negetive or positive + check for invalid - bool done right
    //we will check.. if amount is negetive then check for negetive if amount is positive just write it
    int get_account_id();
    int get_password();
    int get_current_balance();
    bool check_password(int other_password); //TODO: this action will be before all the other because found in 2 - 6
    pthread_mutex_t lock_write;
    pthread_mutex_t lock_read;

private:
    int account_id;
    int password;
    int current_balance;

};
//trade offs - if the account print so if it is lock for the action then we are consistent, but we cant do another action
// if the atm print then the lock will be harder and maybe the print will not be fine
// if the bank locks and print then maybe we can handle the lock perfectly



#endif //OS_HW_NIR_ROY_ACCOUNT_H
