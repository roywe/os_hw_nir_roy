//
// Created by USER on 7/5/2024.
//

#include "account.h"
using namespace std;

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
int Account::deposit(int amount){
    this->current_balance = this->current_balance + amount;
    return current_balance;
}

int Account::withdrawn(int amount){
    if (this->current_balance - amount < 0){
        return -1;
    }
    else{
        this->current_balance = this->current_balance - amount;
        return this->current_balance;
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

int Account::withdrawn_by_per(float randomPer){
    int commission = this->current_balance * randomPer/ 100;
    this->current_balance = this->current_balance - commission;
    return commission;
}

void Account::print_account() const{
    cout << "Account "<< this->account_id << ": Balance - "<< this->current_balance << " $, Account Password - "<< this->password << endl;
}