//
// Created by USER on 7/6/2024.
//

#include "atm_machine.h"
#include <string>
#define DEBUG 0

using namespace std;
std::ofstream log;
extern pthread_mutex_t log_mutex;

ATM::ATM(int atm_id, Bank * bank, std::string file_name){
    this->atm_id = atm_id;
    // we want to seperate between the part of initializing atm and running it therefore I think commands is fine idea
    // otherwise we should think how to return if the file is corrupted before running
    std::ifstream file1_content(file_name);
    string line_file1, tmp;
    while(getline(file1_content,line_file1)){
        tmp = line_file1;
        this->commands.push_back(tmp);
    }
    file1_content.close();
    this->bank = bank;
}


bool ATM::write_log_if_no_account(int acc){
	/**
     * @brief - check if no account and write to log
     * @param acc - account id to check
     * @return false if no account, true if account exists
    */
	if (bank->accounts.find(acc) == bank->accounts.end()) {
		string msg = "Your transaction failed – account id " + to_string(acc) + " does not exist";
		write_msg_to_log(msg, true);
		return false;
	}
	return true;
}

bool ATM::write_log_if_password_not_match(int acc, int password){
	/**
     * @brief - check if no password doesnt match and write to log
     * @param acc - account id to check
     * @param password - password to check
     * @return false if no account, true if account exists
    */
	if (bank->accounts[acc].get_password() != password) {
		string msg = "Your transaction failed – password for account id " + to_string(acc) + " is incorrect";
		write_msg_to_log(msg, true);
		return false;
	}
	return true;
}

int ATM::run(){

    for (size_t i = 0; i < this->commands.size(); ++i) {
    	usleep(100 * 1000); // sleep 100ms between commands
		std::vector <std::string> tempStr = splitString(commands[i], ' ');
		std::string current_command;

		string args[5] = {""};
		for (size_t j = 0; j < tempStr.size(); ++j) {
			args[j] = tempStr[j];

		}

		//till here is the args start
		const char *cmd = args[0].c_str();
		int account_id = atoi(args[1].c_str());
		int password = atoi(args[2].c_str());
		if (!strcmp(cmd, "O")) {
			open_account(account_id, password, atoi(args[3].c_str()));
		} else {
			//maybe the wise thing will be to open also delete account lock - in which only delete check if one is checking it right now

			if (!strcmp(cmd, "D")) {
				int amount = atoi(args[3].c_str());
				deposit(account_id, password, amount);
			} else if (!strcmp(cmd, "W")) {
				int amount = atoi(args[3].c_str());
				withdraw(account_id, password, amount);

			} else if (!strcmp(cmd, "B")) {
				check_balance(account_id, password);
			} else if (!strcmp(cmd, "Q")) {
				close_account(account_id, password);
			} else if (!strcmp(cmd, "T")) {
				int target_account = atoi(args[3].c_str());
				int amount = atoi(args[4].c_str());
				transfer(account_id, password, target_account, amount);
			} else {
				//we can assume that every command is legal by the question...
				printf("%s", "");
			}

		}

	}

    return 1;

}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = str.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start));

    return tokens;

}


void ATM::open_account(int acc_num, int password, int balance){
	/**
     * @brief - open account
     * @param acc - account id of the account
     * @param password - password of the account
     * @param balance - balance of the account
    */
	// open account on bank - need to lock bank for that because we are writing to bank
	string msg;
	bank->write_lock();
	if(bank->accounts.find(acc_num) != bank->accounts.end()){

		msg = "Your transaction failed – account with the same id exists";
		write_msg_to_log(msg, true);

		bank->write_unlock();
		if (DEBUG == 0) sleep(1);
		return;
	}
	bank->accounts[acc_num] = Account(acc_num,password,balance);
	msg = "New account id is " + to_string(acc_num) + " with password " + ((password < 1000) ? "0" + std::to_string(password) : std::to_string(password)) + " and initial balance " + to_string(balance);
	write_msg_to_log(msg, false);
	bank->write_unlock();
	if (DEBUG == 0) sleep(1);


}

int ATM::deposit (int acc_num, int password, int amount ){
	/**
     * @brief - deposit for account
     * lock write for account - read for bank
     * when writing to account bank cant read or write - nobody can read/write to account
     * @param acc - account id of the account
     * @param password - password for checking the account
     * @param amount - amount we will deposit
    */
	bank->read_lock();
	bool is_account = write_log_if_no_account(acc_num);

	if (not is_account){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}
	bool is_password = write_log_if_password_not_match(acc_num, password);
	if (not is_password){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	else{
		bank->accounts[acc_num].write_lock();

		int current_balance = bank->accounts[acc_num].deposit(amount);

		string msg = "Account " + to_string(acc_num) + " new balance is " + to_string(current_balance) + " after " + to_string(amount) + "$ was deposited";
		write_msg_to_log(msg, false);
		bank->accounts[acc_num].write_unlock();
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return SUCCESS;
	}

}

int ATM::withdraw (int acc_num, int password, int amount) {
	/**
     * @brief - withdraw for account
     * lock write for account - read for bank
     * when writing to account bank cant read or write - nobody can read/write to account
     * @param acc - account id of the account
     * @param password - password for checking the account
     * @param amount - amount we will withdraw
    */

	if (amount<0){
		return FAIL;
	}
	string msg;
	bank->read_lock();
	bool is_account = write_log_if_no_account(acc_num);

	if (not is_account) {
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	bool is_password = write_log_if_password_not_match(acc_num, password);
	if(not is_password){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	else{
		bank->accounts[acc_num].write_lock();

		int current_balance = bank->accounts[acc_num].withdrawn(amount);
		if(current_balance == -1){
			msg = "Your transaction failed – account id " + to_string(acc_num) + " balance is lower than " + to_string(amount);
			write_msg_to_log(msg, true);
			bank->accounts[acc_num].write_unlock();
			bank->read_unlock();
			if (DEBUG == 0) sleep(1);
			return FAIL;
		}
		else{
			msg = "Account " + to_string(acc_num) + " new balance is " + to_string(current_balance) + " after " + to_string(amount) + "$ was withdrawn";
			write_msg_to_log(msg, false);
			bank->accounts[acc_num].write_unlock();
			bank->read_unlock();
			if (DEBUG == 0) sleep(1);
			return SUCCESS;
		}
	}


}

int ATM::check_balance (int acc_num, int password){
	/**
     * @brief - check_balance for account
     * lock rad for account - read for bank
     * when reading to account everyone can still read (but no write)
     * @param acc - account id of the account
     * @param password - password for checking the account
    */

	bank->read_lock();
	bool is_account = write_log_if_no_account(acc_num);

	if (not is_account) {
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	bool is_password = write_log_if_password_not_match(acc_num, password);
	if (not is_password){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	else{
		bank->accounts[acc_num].read_lock();

		int current_balance = bank->accounts[acc_num].get_current_balance();
		string msg = "Account " + to_string(acc_num) + " balance is " + to_string(current_balance);
		write_msg_to_log(msg, false);
		bank->accounts[acc_num].read_unlock();
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return SUCCESS;
	}


}

int ATM::close_account (int acc_num, int password){
	/**
     * @brief - close_account
     * write lock bank
     * @param acc - account id of the account
     * @param password - password for checking the account
    */

	bank->write_lock();

	bool is_account = write_log_if_no_account(acc_num);

	if (not is_account) {
		bank->write_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	bool is_password = write_log_if_password_not_match(acc_num, password);
	if(not is_password){
		bank->write_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}
	else{
		auto it = bank->accounts.find(acc_num);

		int temp_balance =  it->second.get_current_balance();
		bank->accounts.erase(it);
		string msg = "Account " + to_string(acc_num) + " is now closed. Balance was " + to_string(temp_balance);
		write_msg_to_log(msg, false);
		bank->write_unlock();
		if (DEBUG == 0) sleep(1);
		return SUCCESS;
	}

}

//TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)
// didnt have a lock or something but maybe
int ATM::transfer (int source_acc, int password,int dest_acc, int amount ){
	/**
     * @brief - transfer
     * read lock bank, lock accounts by their ids values
     * @param source_acc - account id of the source account (withdraw from him)
     * @param password - password for checking the source account
     * @param dest_acc - target account to deposit the money
     * @param amount - amount to transfer between accounts
    */

	if (amount<0 || source_acc == dest_acc){
		return FAIL;
	}

	string msg;

	bank->read_lock();

	bool is_account = write_log_if_no_account(source_acc);
	if (not is_account) {
		//TODO: print just one of the errors by splitting if..
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}
	bool is_password = write_log_if_password_not_match(source_acc, password);
	if(not is_password){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	bool is_account_b = write_log_if_no_account(dest_acc);
	if(not is_account_b){
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	bank->accounts[source_acc].lock_ww_same_order(bank->accounts[dest_acc]);

	int current_balance_a = bank->accounts[source_acc].withdrawn(amount);
	if (current_balance_a == -1 ){
		msg = "Your transaction failed – account id " + to_string(source_acc) + " balance is lower than " + to_string(amount);
		write_msg_to_log(msg, true);
		bank->accounts[source_acc].unlock_ww_same_order(bank->accounts[dest_acc]);
		bank->read_unlock();
		if (DEBUG == 0) sleep(1);
		return FAIL;
	}

	int current_balance_b = bank->accounts[dest_acc].deposit(amount);
	msg = "Transfer "+ to_string(amount) +" from account "+ to_string(source_acc) +" to account "+to_string(dest_acc) + " new account balance is "+ to_string(current_balance_a) + " new target account balance is "+to_string(current_balance_b);
	write_msg_to_log(msg, false);
	bank->accounts[source_acc].unlock_ww_same_order(bank->accounts[dest_acc]);
	bank->read_unlock();
	if (DEBUG == 0) sleep(1);
	return SUCCESS;


}


void ATM::write_msg_to_log(string msg, bool is_error){
	/**
     * @brief - write_msg_to_log
     * lock log
     * @param msg - msg we will write to log
     * @param is_error - indicate if the write is for error (for format issues)
    */

	pthread_mutex_lock(&(log_mutex));
	log.open(log_file, std::ios::out | std::ios::app);
	if(log.is_open()){
		if (is_error){
			log << "Error " << this->atm_id << ": " << msg << endl;
		}

		else{
			log << this->atm_id << ": " << msg << endl;
		}
		log.close();
	}
	pthread_mutex_unlock(&(log_mutex));
}

