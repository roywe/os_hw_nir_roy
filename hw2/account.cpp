//
// Created by USER on 7/5/2024.
//

#include "account.h"


Account::Account(){

}
Account::Account(int account_id, int password, int current_balance){
    this->account_id = account_id;
    this->password = password;
    this->current_balance = current_balance;
}
Account::Account(const Account& other){
    this->account_id = other.account_id;
    this->password = other.password;
    this->current_balance = other.current_balance;
}
Account::~Account(){

}
bool Account::change_balance(int amount){
    if (amount >= 0){
        this->current_balance = this->current_balance + amount;
        return true;
    }
    else{
        if (this->current_balance + amount < 0){
            return false;
        }
        else{
            this->current_balance = this->current_balance + amount;
            return true;
        }
    }
}

int Account::get_account_id(){
    return this->account_id;
}
int Account::get_password(){
    return this->password;
}
int Account::get_current_balance(){
    return this->current_balance;
}
bool Account::check_password(int other_password){
    return this->password == other_password;
}