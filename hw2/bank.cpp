//
// Created by USER on 7/6/2024.
//

#include "bank.h"
#include "atm_machine.h"
using namespace std;
std::string log_file="log.txt";

Bank::Bank(){
	this->bank_lock = ReadWriteLock();
}

Bank::~Bank(){

}

void Bank::lower_random_balance(){
    //TODO: lock all the bank
	this->bank_lock.enter_write();

    std::srand(std::time(nullptr));
    float randomPer = std::rand() % 5 + 1;
    printf("per: %f was chosen",randomPer);
    for (auto& pair : this->accounts) {
        int commission = pair.second.withdrawn_by_per(randomPer);
        log << "Bank: commissions of " <<  randomPer<< " % were charged, bank gained "<<commission << " from account "<< pair.first<<endl;
    }

	this->bank_lock.leave_write();
    //TODO: Unlock all the bank

}
// it happened each 3 s  (locking all accounts) - we will need thread for this - should lock all accounts

void Bank::print_all_accounts(){

	this->bank_lock.enter_read();
    for (const auto& pair : this->accounts) {
        pair.second.print_account();
    }
	this->bank_lock.leave_read();

} // it happened each 0.5 s  (locking all accounts) - we will need thread for this



int main (int argc, char *argv[]) {
	//checking arguments
	if ( argc == 1) {
		cerr << "Bank error: illegal arguments" << endl ;
        exit(1);
	}

    Bank bank = Bank();

    bool file_corrupted = false;
    for (int i=1;i<argc;i++){
        std::ifstream file1_content(argv[i]);
        if (!file1_content.good()){

            file_corrupted = true;
        }
        file1_content.close();
    }
    if (file_corrupted){
        std::cerr << "Error opening input file!" << std::endl;
        exit(1);
    }
    std::ofstream log;
    log.open(log_file);
	if (!log) {
		std::cerr << "Error opening file: " << log_file << std::endl;
		return 1;
	}


    std::string atm_command_file = argv[1];
    ATM atm0 = ATM(0, bank, atm_command_file);
    atm0.run();
    log.close();

}

void Bank::read_lock(){
	cout << "read lock bank" << endl;
	this->bank_lock.enter_read();
}
void Bank::read_unlock(){
	cout << "read unlock bank" << endl;
	this->bank_lock.leave_read();
}
void Bank::write_lock(){
	cout << "write lock bank" << endl;
	this->bank_lock.enter_write();
}
void Bank::write_unlock(){
	cout << "write unlock bank" << endl;
	this->bank_lock.leave_write();
}