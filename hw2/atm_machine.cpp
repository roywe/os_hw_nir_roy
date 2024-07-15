//
// Created by USER on 7/6/2024.
//

#include "atm_machine.h"
#include <string>
using namespace std;
std::ofstream log;
ATM::ATM(int atm_id, Bank bank, std::string file_name){
	log.open(log_file, std::ios::out | std::ios::app);
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
		    std::vector<std::string> tempStr = splitString(commands[i], ' ');
		    std::string current_command;

		    string args[5]= {""};
//	        string cmd = tempStr[0];
		for (size_t j = 0; j < tempStr.size(); ++j) {
			args[j] = tempStr[j];

		}

        //till here is the args start
        const char* cmd = args[0].c_str();
        int account_id = atoi(args[1].c_str());
        int password = atoi(args[2].c_str());
//        this->bank.print_all_accounts();
        if (!strcmp(cmd, "O")){
        	open_account(account_id,password, atoi(args[3].c_str()));
//        	this->bank.add_account(this->atm_id, account_id, password, atoi(args[3].c_str()));
        }
//        else{
//			if(not this->bank.check_password(this->atm_id, account_id, password)){
//                log << "Error " << this->atm_id <<": Your transaction failed – password for account id " << account_id << " is incorrect" <<endl;
//            }

			else if (!strcmp(cmd, "D")){
//				deposit (int acc_num, int password, int amount )
                int amount = atoi(args[3].c_str());
				deposit(account_id,password,amount);
//                int balance_after = this->bank.deposit(this->atm_id, account_id, amount);
//                Account <id> new balance is <balance> after <amount> $ was deposited
//                log << this->atm_id<< ": Account " << account_id << " new balance is " << balance_after <<" after " << amount << " $ was deposited" <<endl;
			}
			else if (!strcmp(cmd, "W")){
                int amount = atoi(args[3].c_str());
//                int balance_after = this->bank.withdrawn(this->atm_id, account_id, amount);
                withdraw(account_id, password,amount);
//                if (balance_after < 0){
//                    log << "Error " << this->atm_id <<": Your transaction failed - account id "<< account_id<< " balance is lower than " << amount << endl;
//                }
//                else{
//                    log << this->atm_id << ": Account " << account_id << " new balance is " << balance_after <<" after " << amount << " $ was withdrawn" <<endl;
//                }
//                Account <id> new balance is <balance> after <amount> $ was deposited

			}
			else if (!strcmp(cmd, "B")){
				check_balance (account_id,password);
//                int balance = this->bank.show_balance(this->atm_id, account_id);
//                log << this->atm_id << ": Account " << account_id << " balance is " << balance<<endl;
			}
			else if (!strcmp(cmd, "Q")){
//                int balance = this->bank.delete_account(this->atm_id, account_id);
//                d close_account (acc_num, int password)
//                log << this->atm_id << ": Account " << account_id << " is now closed. Balance was " << balance<<endl;
				close_account(account_id,password);
			}
			else if (!strcmp(cmd, "T")){
                int target_account = atoi(args[3].c_str());
                int amount = atoi(args[4].c_str());
                transfer (account_id,password,target_account,amount);
//                this->bank.move_between_accounts(this->atm_id, account_id,target_account, amount);
			}
			else{
				//TODO: we can assume that every command is legal by the question...
				printf("%s","wrong command");
			}

        }


    this->bank.print_all_accounts();
    this->bank.lower_random_balance();
    this->bank.print_all_accounts();

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
	if(bank.accounts.find(acc_num) != bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account with the same id exists" << endl;
		return;
	}
	bank.accounts[acc_num] = Account(acc_num,password,balance);
//	bank.insert_account(new_account);
	log << this->atm_id <<": New account id is " << acc_num << " with password " << password << " and initial balance " << balance << endl;

}
void ATM::deposit (int acc_num, int password, int amount ){
	if(bank.accounts.find(acc_num) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password() != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	bank.accounts[acc_num].deposit(amount);
	log << this->atm_id << ": Account " << acc_num <<" new balance is " << bank.accounts[acc_num].get_current_balance() <<" after " << amount << "$ was deposited" << endl;
}

void ATM::withdraw (int acc_num, int password, int amount ){
	if(bank.accounts.find(acc_num) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password() != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_current_balance() < amount){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num << " balance is lower than " << amount <<endl;
		return;
	}
	bank.accounts[acc_num].withdrawn(amount);
	log << this->atm_id << ": Account " << acc_num <<" new balance is " << bank.accounts[acc_num].get_current_balance() <<" after " << amount << "$ was withdrawn" << endl;


}
void ATM::check_balance (int acc_num, int password){
	if(bank.accounts.find(acc_num) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password() != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	log << this->atm_id << ": Account " << acc_num <<" balance is " << bank.accounts[acc_num].get_current_balance() << endl;

}

void ATM::close_account (int acc_num, int password){
	if(bank.accounts.find(acc_num) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password() != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	int temp_balance =  bank.accounts[acc_num].get_current_balance();
	bank.accounts.erase(acc_num);
	log << this->atm_id << ": Account " << acc_num <<" is now closed. Balance was " << temp_balance << endl;

}
void ATM::transfer (int source_acc, int password,int dest_acc, int amount ){
	if(bank.accounts.find(source_acc) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << source_acc <<" does not exist" << endl;
		return;
	}
	if(bank.accounts.find(dest_acc) == bank.accounts.end()){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << dest_acc <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[source_acc].get_password() != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << source_acc << " is incorrect" << endl;
		return;
	}
	if(bank.accounts[source_acc].get_current_balance() < amount){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << source_acc << " balance is lower than " << amount <<endl;
		return;
	}
	bank.accounts[source_acc].withdrawn(amount);
	bank.accounts[dest_acc].deposit(amount);
	log <<this->atm_id << ": Transfer "<< amount <<" from account "<<source_acc <<" to account "<<source_acc <<" new account balance is "<<bank.accounts[source_acc].get_current_balance() <<" new target account balance is "<<bank.accounts[dest_acc].get_current_balance() << endl;



}
