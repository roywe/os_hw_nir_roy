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
    this->rw_account = ReadWriteLock();
}

//Account::Account(const Account& other){
//    this->account_id = other.account_id;
//    this->password = other.password;
//    this->current_balance = other.current_balance;
//}

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

void Account::read_lock(){
    cout << "read lock account: " << this->account_id << endl;
    this->rw_account.enter_read();
}
void Account::read_unlock(){
    cout << "read unlock account: " << this->account_id << endl;
    this->rw_account.leave_read();
}
void Account::write_lock(){
    cout << "write lock account: " << this->account_id << endl;
    this->rw_account.enter_write();
}
void Account::write_unlock(){
    cout << "write unlock account: " << this->account_id << endl;
    this->rw_account.leave_write();
}


void Account::lock_ww_same_order(Account& other){
	if (other.account_id>this->account_id){
		this->rw_account.enter_write();
		other.rw_account.enter_write();
	}
	else{
		other.rw_account.enter_write();
		this->rw_account.enter_write();
	}
}

void Account::unlock_ww_same_order(Account& other){
	if (other.account_id>this->account_id){
		other.rw_account.leave_write();
		this->rw_account.leave_write();
	}
	else{
		this->rw_account.leave_write();
		other.rw_account.leave_write();
	}
}