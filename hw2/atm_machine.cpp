//
// Created by USER on 7/6/2024.
//

#include "atm_machine.h"
using namespace std;
ATM::ATM(int atm_id, Bank& bank,std::string file_name, std::string log_file){
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
    this->log_file = log_file
    this->bank = bank

//TODO: this part will be at the parsing arguments in the bank main - atm will get valid(!) file
//    if (!file1_content.good()){
//        //need to save state of the atm
//    }
//    else{
//
//    }


}

ATM::run(){
    size_t atm_command_index = 0;

    const char* delimiters = " \t\n";
    std::string current_command;

    while (atm_command_index < this->commands.size()){
        atm_command_index++;
        current_command = this->commands[atm_command_index];

        int i = 0, num_arg = 0;
        char* args[5];
        //this is the max args - we need that for each command to get the arguments, so maybe it will be better to create function that return char*
        //doing the rows below
        cmd = strtok(current_command, delimiters);
        if (cmd == NULL)
            printf("%s","handle void command");
            //TODO: handle wrong cmd - by the question it is fine but for start to see that it is working for us we can use it
        args[0] = cmd;
        for (i=1; i<MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
            if (args[i] != NULL)
                num_arg++;
        }
        //till here is the args start

        if (!strcmp(cmd, "O")){

        }
        else{
            int account_id = atoi(args[1]);
            int password = atoi(args[2]);
			if (!strcmp(cmd, "O")){
				open_account(account_id,password,atoi(args[3]));
			}
			else if (!strcmp(cmd, "D")){
				deposit(account_id,password,atoi(args[3]));
			}
			else if (!strcmp(cmd, "W")){
				withdraw(account_id,password,atoi(args[3]));
			}
			else if (!strcmp(cmd, "B")){
				check_balance(account_id,password);
			}
			else if (!strcmp(cmd, "Q")){
				close_account(account_id,password);
			}
			else if (!strcmp(cmd, "T")){
				transfer(account_id,password,atoi(args[3]),atoi(args[4]));
			}
			else{
				//TODO: we can assume that every command is legal by the question...
				printf("%s","wrong command");
			}



        }

    }
    return 1;

}


void open_account(int acc_num, int password, int balance){
	if(bank.accounts[acc_num] != NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account with the same id exists" << endl;
		return;
	}
	Account new_account = account(acc_num,password,balance);
	bank.insert_account(new_account);
	log << this->atm_id <<": New account id is " << acc_num << " with password " << password << " and initial balance " << balance << endl;

}
void deposit (int acc_num, int password, int amount ){
	if(bank.accounts[acc_num] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	bank.accounts[acc_num].change_balance(amount);
	log << this->atm_id << ": Account " << acc_num <<" new balance is " << bank.accounts[acc_num].get_current_balance() <<" after " << amount << "$ was deposited" << endl;
}

void withdraw (int acc_num, int password, int amount ){
	if(bank.accounts[acc_num] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_current_balance < amount){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num << " balance is lower than " << amount <<endl;
		return;
	}
	bank.accounts[acc_num].change_balance(-amount);
	log << this->atm_id << ": Account " << acc_num <<" new balance is " << bank.accounts[acc_num].get_current_balance() <<" after " << amount << "$ was withdrawn" << endl;


}
void check_balance (int acc_num, int password){
	if(bank.accounts[acc_num] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	log << this->atm_id << ": Account " << acc_num <<" balance is " << bank.accounts[acc_num].get_current_balance() << endl;

}

void close_account (int acc_num, int password){
	if(bank.accounts[acc_num] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << acc_num <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[acc_num].get_password != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << acc_num << " is incorrect" << endl;
		return;
	}
	int temp_balance =  bank.accounts[acc_num].get_current_balance();
	bank.accounts.remove(acc_num);
	log << this->atm_id << ": Account " << acc_num <<" is now closed. Balance was " << temp_balance << endl;

}
void transfer (int source_acc, int password,int dest_acc, int amount ){
	if(bank.accounts[source_acc] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << source_acc <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[dest_acc] == NULL){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << dest_acc <<" does not exist" << endl;
		return;
	}
	if(bank.accounts[source_acc].get_password != password){
		log << "Error" << this->atm_id << ": Your transaction failed – password for account id " << source_acc << " is incorrect" << endl;
		return;
	}
	if(bank.accounts[source_acc].get_current_balance < amount){
		log << "Error" << this->atm_id << ": Your transaction failed – account id " << source_acc << " balance is lower than " << amount <<endl;
		return;
	}
	bank.accounts[source_acc].change_balance(-amount);
	bank.accounts[dest_acc].change_balance(amount);
	log <<this->atm_id << ": Transfer "<< amount <<" from account "<<source_acc <<" to account "<<source_acc <<" new account balance is "<<bank.accounts[source_acc].get_current_balance <<" new target account balance is "<<bank.accounts[source_acc].get_current_balance << endl;



}
