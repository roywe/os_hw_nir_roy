//
// Created by USER on 7/6/2024.
//

#include "atm_machine.h"
#include <string>
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





int ATM::run(){
//    size_t atm_command_index = 0;


    for (size_t i = 0; i < this->commands.size(); ++i) {
//        std::cout <<"Num :" << i << " is: "<<commands[i] << endl;
		std::vector <std::string> tempStr = splitString(commands[i], ' ');
		std::string current_command;

		string args[5] = {""};
//	        string cmd = tempStr[0];
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
			if (bank->accounts.find(account_id) == bank->accounts.end()) {
				log << "Error" << this->atm_id << ": Your transaction failed – account id " << account_id
					<< " does not exist" << endl;
//				return 0;
			}

			else if (bank->accounts[account_id].get_password() != password) {
				log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << account_id
					<< " is incorrect" << endl;
//				return 0;
			} else if (!strcmp(cmd, "D")) {
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
				//TODO: we can assume that every command is legal by the question...
				printf("%s", "wrong command");
			}

		}

	}
    this->bank->print_all_accounts();
    this->bank->lower_random_balance();
    this->bank->print_all_accounts();

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
	// open account on bank - doesn't interupt anything
	string msg;
	bank->write_lock();
	if(bank->accounts.find(acc_num) != bank->accounts.end()){

		msg = "Your transaction failed – account with the same id exists";
		write_msg_to_log(msg, true);
		bank->write_unlock();
		return;
	}
	bank->accounts[acc_num] = Account(acc_num,password,balance);
	bank->write_unlock();
	msg = "New account id is " + to_string(acc_num) + " with password " + to_string(password) + " and initial balance " + to_string(balance);
	write_msg_to_log(msg, false);

}

//TODO - locking the account id - lock account read+write mutex + bank_lock
//TODO - seems fine but had problems - when putting 2 machines with same pass the logs become not good
// it might be beacuse of the bank commissions
void ATM::deposit (int acc_num, int password, int amount ){
	//lock write for account
	//	when writing to account bank cant read or write - nobody can write to account
	bank->write_lock();
	bank->accounts[acc_num].write_lock();

	bank->accounts[acc_num].deposit(amount);
	string msg = "Account " + to_string(acc_num) + " new balance is " + to_string(bank->accounts[acc_num].get_current_balance()) + " after " + to_string(amount) + "$ was deposited";
	write_msg_to_log(msg, false);

	bank->accounts[acc_num].write_unlock();
	bank->write_unlock();

	//lock write for account
}

//TODO - locking the account id - lock account read+write mutex + bank_lock
// fix locking - has probelm for 2 threads
void ATM::withdraw (int acc_num, int password, int amount) {
	//lock write for account - withdraw - nobody can write to this account etc

	string msg;
	bank->write_lock();
	bank->accounts[acc_num].write_lock();

    if(bank->accounts[acc_num].get_current_balance() < amount){
		msg = "Your transaction failed – account id " + to_string(acc_num) + " balance is lower than " + to_string(amount);
		write_msg_to_log(msg, true);
		bank->accounts[acc_num].write_unlock();
		bank->write_lock();
        return;
    }
	else{
		bank->accounts[acc_num].withdrawn(amount);
		msg = "Account " + to_string(acc_num) + " new balance is " + to_string(bank->accounts[acc_num].get_current_balance()) + " after " + to_string(amount) + "$ was withdrawn";
		write_msg_to_log(msg, false);

		bank->accounts[acc_num].write_unlock();
		bank->write_unlock();
		return;
	}



}




//TODO: need to lock account - lock account write mutex..
// seems fine and I didnt see any lock need to check further
void ATM::check_balance (int acc_num, int password){

	bank->read_lock();
	bank->accounts[acc_num].read_lock();

	string msg = "Account " + to_string(acc_num) + " balance is " + to_string(bank->accounts[acc_num].get_current_balance());

	write_msg_to_log(msg, false);

	bank->accounts[acc_num].read_unlock();
	bank->read_unlock();

}

//TODO: need to lock account because if moving amount the it is not good - lock account write+read mutex + bank_lock
//TODO: check that actually erase and return answer if not...
//need to think what to lock if we delete and someone already got user...
void ATM::close_account (int acc_num, int password){
	//lock bank, lock user somehow
	bank->write_lock();
//	bank.write_lock();
	int temp_balance =  bank->accounts[acc_num].get_current_balance();
	bank->accounts.erase(acc_num);
	string msg = "Account " + to_string(acc_num) + " is now closed. Balance was " + to_string(temp_balance);
	write_msg_to_log(msg, false);

	bank->write_unlock();
}

//TODO:   implement lock_account_by_order() //is it before we get them or after... because they can be deleted - maybe adding anothe lock will help
//getting 2 accounts and locking by account id, another option - get mapping and lock..
//we need the find to be somehow atomic - maybe external lock will help for this case
//TODO: need to lock account a from reading+writing+bank_lock and lock the other from reading - sort the locks by ids size (to avoid deadlock)
// didnt have a lock or something but maybe
void ATM::transfer (int source_acc, int password,int dest_acc, int amount ){

	string msg;
	if(bank->accounts.find(dest_acc) == bank->accounts.end()){
		msg = "Your transaction failed – account id " + to_string(dest_acc) + " does not exist";
		write_msg_to_log(msg, true);
		return;
	}
	if(bank->accounts[source_acc].get_current_balance() < amount){
		msg = "Your transaction failed – account id " + to_string(source_acc) + " balance is lower than " + to_string(amount);
		write_msg_to_log(msg, true);
		return;
	}

	bank->write_lock();

	bank->accounts[source_acc].lock_ww_same_order(bank->accounts[dest_acc]);

	bank->accounts[source_acc].withdrawn(amount);
    //TODO: need to check balance somehow
	bank->accounts[dest_acc].deposit(amount);
	msg = "Transfer "+ to_string(amount) +" from account "+ to_string(source_acc) +" to account "+to_string(dest_acc) + " new account balance is "+ to_string(bank->accounts[source_acc].get_current_balance()) + " new target account balance is "+to_string(bank->accounts[dest_acc].get_current_balance());

	write_msg_to_log(msg, false);
	bank->accounts[source_acc].read_unlock();

	bank->accounts[source_acc].unlock_ww_same_order(bank->accounts[dest_acc]);
	bank->write_unlock();
}


void ATM::write_msg_to_log(string msg, bool is_error){
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

// find add
// invalidate acc
// delete acc

// כשאני מחזיק אותו שאף אחד לא יוכל בכלל למצוא אותו

// טרם המחיקה

// percant - int - result - 2 digits after dot
// commission of the bank -