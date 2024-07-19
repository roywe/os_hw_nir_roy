//
// Created by USER on 7/6/2024.
//

#include "bank.h"
#include "atm_machine.h"
using namespace std;
std::string log_file="log.txt";
pthread_mutex_t log_mutex;
#define DEBUG 0

Bank::Bank(){
	this->bank_lock = ReadWriteLock();
}

Bank::~Bank(){

}

//TODO: locks of the lower balance and print are not good for the log for example - need to check for atm
void Bank::lower_random_balance(){
    //TODO: lock all the bank
	this->bank_lock.enter_write();

    std::srand(std::time(nullptr));
    float randomPer = std::rand() % 5 + 1;
    printf("per: %f was chosen",randomPer);
    for (auto& pair : this->accounts) {
        int commission = pair.second.withdrawn_by_per(randomPer);
        cout << "Bank: commissions of " <<  randomPer<< " % were charged, bank gained "<<commission << " from account "<< pair.first<<endl;
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




void* work_wrap(void* atm) {
    ATM* atm_ptr = static_cast<ATM*>(atm);
    atm_ptr->run();
	//pthread_detach(pthread_self());
    pthread_exit(NULL);
}

void* print_wrap(void* bank) {
    Bank* bankPtr = static_cast<Bank*>(bank);
    bankPtr->print_all_accounts();
    pthread_exit(NULL);
	//pthread_detach(pthread_self());
}

void* take_comm(void* bank) {
    Bank* bankPtr = static_cast<Bank*>(bank);
    bankPtr->lower_random_balance();
    pthread_exit(NULL);
}

void Bank::read_lock(){
	if (DEBUG == 1) cout << "read lock bank" << endl;
	else this->bank_lock.enter_read();
}
void Bank::read_unlock(){
	if (DEBUG == 1) cout << "read unlock bank" << endl;
	else this->bank_lock.leave_read();
}
void Bank::write_lock(){
	if (DEBUG == 1) cout << "write lock bank" << endl;
	else this->bank_lock.enter_write();
}
void Bank::write_unlock(){
	if (DEBUG == 1) cout << "write unlock bank" << endl;
	else this->bank_lock.leave_write();
}


int main (int argc, char *argv[]) {
	//checking arguments

	if ( argc == 1) {
		cerr << "Bank error: illegal arguments" << endl ;
        exit(1);
	}


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
	log.close();

	Bank bank = Bank();
	pthread_t *atm_threads = new pthread_t[argc -1];
	pthread_t comission_threads;
	pthread_t print_thread;
	vector <ATM> atm_arr;
	for (int i = 0; i<argc -1; ++i){
		ATM atm =ATM(i,&bank,argv[i+1]);
		atm_arr.push_back(atm);
//		cout << "print here1" << endl;
	}

	for (int i = 0 ; i < argc-1 ; i++) {
//		cout << "print here2" << endl;
		if ( pthread_create(&atm_threads[i] , NULL ,work_wrap , &(atm_arr[i]) ) ){
			perror("Bank error: pthread_create failed");
			delete[] atm_threads;
			return 1;
		}
	}
	if (pthread_create(&comission_threads, NULL, take_comm, &bank)){
		perror("Bank error: pthread create failed");
		delete[] atm_threads;
		return 1;
	}
	if (pthread_create(&print_thread, NULL, print_wrap, &bank)){
		perror("Bank error: pthread create failed");
		delete[] atm_threads;
		return 1;
	}

	for (int i = 0; i<argc -1; ++i){
//		cout << "print here3" << endl;
		pthread_join(atm_threads[i], NULL);
	}

//	cout << "print here" << endl;
//	log<< "test" << endl;

//    std::string atm_command_file = argv[1];
//    ATM atm0 = ATM(0, bank, atm_command_file);
//    atm0.run();
//    log.close();
	delete[] atm_threads;
	return 0;

}